#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <wlanapi.h>
#include <fstream>
#include <sstream>
#include "CommonData.h"
#include "CommonLibrary.h"

namespace network_lib
{
    std::vector<lan_info_t> listLanInfo = {}; // Cache for detected LAN/Ethernet adapters
    std::vector<wifi_info_t> listWiFiInfo = {}; // Cache for scanned WiFi networks

    // Scans the system for all active network adapters (Ethernet and WiFi)
    void GetLANConnections()
    {
        listLanInfo.clear();

        unsigned long outBufLen = 0;
        PIP_ADAPTER_ADDRESSES pAddresses = NULL;

        // Determine the buffer size required to store adapter information
        DWORD dwRetVal = ::GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen);

        if (dwRetVal == ERROR_BUFFER_OVERFLOW)
        {
            pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen); // Allocate sufficient memory
        }

        if (pAddresses != NULL)
        {
            // Fetch the actual adapter data into the allocated buffer
            dwRetVal = ::GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen);

            if (dwRetVal == NO_ERROR)
            {
                PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;

                while (pCurrAddresses)
                {
                    // Filter for common networking interfaces (Ethernet and WiFi)
                    if (pCurrAddresses->IfType == IF_TYPE_ETHERNET_CSMACD || pCurrAddresses->IfType == IF_TYPE_IEEE80211)
                    {
                        lan_info_t info = {
                            pCurrAddresses->IfType,
                            common_lib::ConvertWStringToString(pCurrAddresses->FriendlyName),
                            common_lib::ConvertWStringToString(pCurrAddresses->Description),
                            pCurrAddresses->OperStatus,
                            pCurrAddresses->Ipv4Metric,
                        };

                        listLanInfo.push_back(info);
                    }

                    pCurrAddresses = pCurrAddresses->Next;
                }
            }

            free(pAddresses);
        }
    }

    // Scans surrounding area for available WiFi SSIDs using Windows Native Wifi API
    void GetWiFiSSIDs()
    {
        listWiFiInfo.clear();

        HANDLE hClient = NULL;
        DWORD dwMaxClient = WLAN_UI_API_VERSION; 
        DWORD dwCurVersion = 0;
        DWORD dwResult = 0;

        // Connect to the WLAN service
        dwResult = ::WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
        if (dwResult != ERROR_SUCCESS)
        {
            return;
        }

        // List installed WiFi adapters
        PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
        dwResult = ::WlanEnumInterfaces(hClient, NULL, &pIfList);
        if (dwResult != ERROR_SUCCESS)
        {
            ::WlanCloseHandle(hClient, NULL);
            return;
        }

        for (DWORD i = 0; i < pIfList->dwNumberOfItems; i++)
        {
            WLAN_INTERFACE_INFO IfInfo = pIfList->InterfaceInfo[i];

            // Request a fresh scan of the radio spectrum
            ::WlanScan(hClient, &IfInfo.InterfaceGuid, NULL, NULL, NULL);

            // Retrieve the network list (Windows may return cached results immediately)
            PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
            dwResult = ::WlanGetAvailableNetworkList(hClient, &IfInfo.InterfaceGuid, WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_MANUAL_HIDDEN_PROFILES, NULL, &pBssList);

            if (dwResult == ERROR_SUCCESS)
            {
                for (DWORD j = 0; j < pBssList->dwNumberOfItems; j++)
                {
                    WLAN_AVAILABLE_NETWORK network = pBssList->Network[j];

                    // Skip hidden/unnamed networks
                    if (network.dot11Ssid.uSSIDLength == 0)
                    {
                        continue;
                    }

                    wifi_info_t info = {
                        std::string((char*)network.dot11Ssid.ucSSID, network.dot11Ssid.uSSIDLength),
                        "", // Password is not provided by scan
                        GetAuthAlgorithmString(network.dot11DefaultAuthAlgorithm),
                        GetCipherAlgorithmString(network.dot11DefaultCipherAlgorithm),
                        network.wlanSignalQuality,
                    };

                    listWiFiInfo.push_back(info);
                }

                // Release the network list memory of this interface
                if (pBssList != NULL)
                {
                    ::WlanFreeMemory(pBssList);
                }
            }
        }

        // Clean up and free up all resources.
        if (pIfList != NULL)
        {
            ::WlanFreeMemory(pIfList);
        }

        ::WlanCloseHandle(hClient, NULL);
    }

    // Resolves IP, Subnet Mask, and Gateway for a specific adapter by its friendly name
    ip_info_t GetNetworkIPByName(std::string targetAdapterName)
    {
        ip_info_t info;

        if (targetAdapterName.empty())
        {
            return info;
        }

        ULONG tableSize = 0;
        ::GetIpAddrTable(NULL, &tableSize, FALSE); // Query required size

        PMIB_IPADDRTABLE pIPAddrTable = (PMIB_IPADDRTABLE)malloc(tableSize);
        if (::GetIpAddrTable(pIPAddrTable, &tableSize, FALSE) != NO_ERROR)
        {
            free(pIPAddrTable);
            return info;
        }

        if (pIPAddrTable)
        {
            for (DWORD i = 0; i < pIPAddrTable->dwNumEntries; i++)
            {
                MIB_IFROW ifRow = {};
                ifRow.dwIndex = pIPAddrTable->table[i].dwIndex;

                if (::GetIfEntry(&ifRow) == NO_ERROR)
                {
                    std::string currentName((char*)ifRow.bDescr);

                    // Check if the adapter description matches our target
                    if (currentName.find(targetAdapterName) != std::string::npos)
                    {
                        // Buffers to store IP strings safely (INET_ADDRSTRLEN = 16 bytes for IPv4)
                        char ipBuffer[INET_ADDRSTRLEN] = { 0 };
                        char maskBuffer[INET_ADDRSTRLEN] = { 0 };

                        struct in_addr ipAddr = {}, maskAddr = {};
                        ipAddr.S_un.S_addr = pIPAddrTable->table[i].dwAddr;
                        maskAddr.S_un.S_addr = pIPAddrTable->table[i].dwMask;

                        // Convert binary IP to human-readable strings
                        if (inet_ntop(AF_INET, &ipAddr, ipBuffer, sizeof(ipBuffer)) != nullptr)
                        {
                            info.ip = ipBuffer;
                        }

                        if (inet_ntop(AF_INET, &maskAddr, maskBuffer, sizeof(maskBuffer)) != nullptr)
                        {
                            info.mask = maskBuffer;
                        }

                        // Determine Gateway by searching the Routing Table for the default route (0.0.0.0) on this interface
                        ULONG routeSize = 0;
                        ::GetIpForwardTable(NULL, &routeSize, FALSE);
                        PMIB_IPFORWARDTABLE pRouteTable = (PMIB_IPFORWARDTABLE)malloc(routeSize);

                        if (pRouteTable)
                        {
                            if (::GetIpForwardTable(pRouteTable, &routeSize, FALSE) == NO_ERROR)
                            {
                                for (DWORD j = 0; j < pRouteTable->dwNumEntries; j++)
                                {
                                    if ((pRouteTable->table[j].dwForwardIfIndex == ifRow.dwIndex)
                                        && (pRouteTable->table[j].dwForwardDest == 0))
                                    {
                                        char gwBuffer[INET_ADDRSTRLEN] = { 0 };
                                        struct in_addr gwAddr = {};
                                        gwAddr.S_un.S_addr = pRouteTable->table[j].dwForwardNextHop;

                                        if (::inet_ntop(AF_INET, &gwAddr, gwBuffer, sizeof(gwBuffer)) != nullptr) {
                                            info.gateway = gwBuffer;
                                        }

                                        break;
                                    }
                                }
                            }
                        }

                        free(pRouteTable);
                        break;
                    }
                }
            }
        }

        free(pIPAddrTable);
        return info;
    }

    // Maps DOT11 authentication enum to a simplified string representation
    std::string GetAuthAlgorithmString(DOT11_AUTH_ALGORITHM auth)
    {
        switch (auth)
        {
        case DOT11_AUTH_ALGO_80211_OPEN:
            return "open";
        case DOT11_AUTH_ALGO_80211_SHARED_KEY:
            return "shared";
        case DOT11_AUTH_ALGO_WPA:
            return "WPA";
        case DOT11_AUTH_ALGO_WPA_PSK:
            return "WPAPSK";
        case DOT11_AUTH_ALGO_WPA_NONE:
            return "WPANone";
        case DOT11_AUTH_ALGO_RSNA:
            return "WPA2";
        case DOT11_AUTH_ALGO_RSNA_PSK:
            return "WPA2PSK";
        case DOT11_AUTH_ALGO_WPA3:
            return "WPA3";
        case DOT11_AUTH_ALGO_WPA3_SAE:
            return "WPA3SAE";
        default:
            return "unknown";
        }
    }

    // Maps DOT11 cipher enum to a simplified string representation
    std::string GetCipherAlgorithmString(DOT11_CIPHER_ALGORITHM cipher)
    {
        switch (cipher)
        {
        case DOT11_CIPHER_ALGO_NONE:
            return "none";
        case DOT11_CIPHER_ALGO_WEP40:
            return "WEP";
        case DOT11_CIPHER_ALGO_TKIP:
            return "TKIP";
        case DOT11_CIPHER_ALGO_CCMP:
            return "AES";
        case DOT11_CIPHER_ALGO_WEP104:
            return "WEP";
        case DOT11_CIPHER_ALGO_WEP:
            return "WEP";
        default:
            return "unknown";
        }
    }

    // Retrieves the NetBIOS name of the current machine
    std::string GetLocalHostName()
    {
        char buffer[MAX_PATH];
        DWORD bufferSize = sizeof(buffer);

        ::GetComputerNameA(buffer, &bufferSize);

        return buffer;
    }

    // Generates a list of strings for UI dropdowns from the LAN adapter list
    std::vector<std::string> CreateNetworkTypeDropdownOptions()
    {
        std::vector<std::string> result;

        for (const auto& info : listLanInfo)
        {
            result.push_back(info.name);
        }

        return result;
    }

    // Generates a list of strings for UI dropdowns from the WiFi scan results
    std::vector<std::string> CreateSSIDDropdownOptions()
    {
        std::vector<std::string> result;

        for (const auto& info : listWiFiInfo)
        {
            result.push_back(info.ssid);
        }

        return result;
    }

    // Connects to a WiFi network by generating and applying a temporary XML profile
    bool ConnectWiFiWithTemplate(const wifi_info_t& info)
    {
        if (info.IsAnyNone())
        {
            return false;
        }

        // Load the XML template file
        std::ifstream templateFile(common_lib::ConvertWStringToString(common_lib::GetFullPath(L"wifi_template.xml")));
        if (!templateFile.is_open())
        {
            return false;
        }

        std::stringstream buffer;
        buffer << templateFile.rdbuf();
        std::string xmlTemplate = buffer.str();
        templateFile.close();

        // Populate the XML template with network credentials
        size_t targetSize = xmlTemplate.size() + (info.ssid.size() * 2) + info.password.size() + info.auth.size() + info.cipher.size() + 1;
        std::vector<char> formattedXml(targetSize);

        int result = snprintf(formattedXml.data(), targetSize, xmlTemplate.c_str(),
            info.ssid.c_str(), info.ssid.c_str(), info.auth.c_str(), info.cipher.c_str(), info.password.c_str());

        if (result < 0 || result >= targetSize)
        {
            return false;
        }

        // Write the populated XML to a temporary file
        std::string tempFileName = common_lib::ConvertWStringToString(common_lib::GetFullPath(L"wifi_ready.xml"));
        std::ofstream outFile(tempFileName);
        if (!outFile.is_open())
        {
            return false;
        }

        outFile << formattedXml.data();
        outFile.close();

        // Use 'netsh' commands to add the profile and initiate connection
        std::string addCmd = "/c netsh wlan add profile filename=\"" + tempFileName + "\" >nul";
        std::string connCmd = "/c netsh wlan connect name=\"" + info.ssid + "\" ssid=\"" + info.ssid + "\"";

        ::ShellExecute(NULL, L"open", L"cmd", common_lib::ConvertStringToWString(addCmd).c_str(), common_lib::GetSystemPath().c_str(), SW_HIDE);
        ::Sleep(TIMECYCLE_100MS);
        ::ShellExecute(NULL, L"open", L"cmd", common_lib::ConvertStringToWString(connCmd).c_str(), common_lib::GetSystemPath().c_str(), SW_HIDE);
        ::Sleep(TIMECYCLE_100MS);

        // Security: Delete the temporary XML containing the clear-text password
        std::string deleteCmd = "del " + tempFileName;
        std::system(deleteCmd.c_str());

        return true;
    }

    // Verifies WiFi connection status via shell command parsing
    bool IsWiFiConnectedTo(std::string targetSsid)
    {
        std::string output = common_lib::ExecuteCMD("netsh wlan show interfaces");

        if (targetSsid == "")
        {
            // Just check for any active connection
            if ((output.find("connected") != std::string::npos)
                && (output.find("disconnected") == std::string::npos))
            {
                return true;
            }
        }
        else
        {
            // Check for connection to a specific SSID
            if ((output.find("connected") != std::string::npos)
                && (output.find("disconnected") == std::string::npos)
                && (output.find(targetSsid) != std::string::npos)
                )
            {
                return true;
            }
        }

        return false;
    }

    // Splits two short integers into four bytes (Octets) for IP representation
    std::vector<short> CalculateIPFromCombinedNum(short part12, short part34)
    {
        std::vector<short> result = {
            static_cast<short>((part12 >> 8) & 0x00ff),
            static_cast<short>((part12 >> 0) & 0x00ff),
            static_cast<short>((part34 >> 8) & 0x00ff),
            static_cast<short>((part34 >> 0) & 0x00ff),
        };

        return result;
    }

    // Converts packed IP shorts into a standard dot-separated string
    std::string CalculateIPStrFromCombinedNum(short part12, short part34)
    {
        std::string result =
            std::to_string(static_cast<short>((part12 >> 8) & 0x00ff)) + "."
            + std::to_string(static_cast<short>((part12 >> 0) & 0x00ff)) + "."
            + std::to_string(static_cast<short>((part34 >> 8) & 0x00ff)) + "."
            + std::to_string(static_cast<short>((part34 >> 0) & 0x00ff));

        return result;
    }
}
