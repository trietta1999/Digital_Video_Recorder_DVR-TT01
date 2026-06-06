#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <wlanapi.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "CommonData.h"
#include "CommonLibrary.h"

namespace network_lib
{
    std::vector<lan_info_t> listLanInfo = {};
    std::vector<wifi_info_t> listWiFiInfo = {};

    void GetLANConnections()
    {
        listLanInfo.clear();

        unsigned long outBufLen = 0;
        PIP_ADAPTER_ADDRESSES pAddresses = NULL;

        // Pass NULL to Windows to return the required size to outBufLen
        DWORD dwRetVal = ::GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen);

        // If the system reports insufficient memory
        if (dwRetVal == ERROR_BUFFER_OVERFLOW)
        {
            // Allocate the exact number of bytes that Windows requires
            pAddresses = (IP_ADAPTER_ADDRESSES*)malloc(outBufLen);
        }

        if (pAddresses != NULL)
        {
            // The buffer has allocated enough space to retrieve the actual data
            dwRetVal = ::GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, pAddresses, &outBufLen);

            if (dwRetVal == NO_ERROR)
            {
                PIP_ADAPTER_ADDRESSES pCurrAddresses = pAddresses;

                while (pCurrAddresses)
                {
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

    void GetWiFiSSIDs()
    {
        listWiFiInfo.clear();

        HANDLE hClient = NULL;
        DWORD dwMaxClient = WLAN_UI_API_VERSION; // WLAN_CLIENT_VERSION_2
        DWORD dwCurVersion = 0;
        DWORD dwResult = 0;

        // Open a connection to the Windows WLAN service
        dwResult = ::WlanOpenHandle(dwMaxClient, NULL, &dwCurVersion, &hClient);
        if (dwResult != ERROR_SUCCESS)
        {
            return;
        }

        // List the Wi-Fi cards (interfaces) installed on the computer
        PWLAN_INTERFACE_INFO_LIST pIfList = NULL;
        dwResult = ::WlanEnumInterfaces(hClient, NULL, &pIfList);
        if (dwResult != ERROR_SUCCESS)
        {
            ::WlanCloseHandle(hClient, NULL);
            return;
        }

        // Browse through each found Wi-Fi card (usually a computer only has one physical Wi-Fi card)
        for (DWORD i = 0; i < pIfList->dwNumberOfItems; i++)
        {
            WLAN_INTERFACE_INFO IfInfo = pIfList->InterfaceInfo[i];

            // Instruct the network card to scan for surrounding Wi-Fi signals
            // This step sends a request to the hardware; Windows will take some time to update
            ::WlanScan(hClient, &IfInfo.InterfaceGuid, NULL, NULL, NULL);

            // Normally, the system needs about 1-2 seconds to receive all signals,
            // but the WlanGetNetworkBssList function below will retrieve the most recently cached network list from Windows,
            // so it can be read immediately

            // Get the list of available Wi-Fi networks (Available Network List)
            PWLAN_AVAILABLE_NETWORK_LIST pBssList = NULL;
            dwResult = ::WlanGetAvailableNetworkList(hClient, &IfInfo.InterfaceGuid, WLAN_AVAILABLE_NETWORK_INCLUDE_ALL_MANUAL_HIDDEN_PROFILES, NULL, &pBssList);

            if (dwResult == ERROR_SUCCESS)
            {
                // Get card info: IfInfo.strInterfaceDescription

                for (DWORD j = 0; j < pBssList->dwNumberOfItems; j++)
                {
                    WLAN_AVAILABLE_NETWORK network = pBssList->Network[j];

                    // Check if the SSID name is blank (invisible network)
                    if (network.dot11Ssid.uSSIDLength == 0)
                    {
                        continue;
                    }

                    wifi_info_t info = {
                        std::string((char*)network.dot11Ssid.ucSSID, network.dot11Ssid.uSSIDLength),
                        "",
                        GetAuthAlgorithmString(network.dot11DefaultAuthAlgorithm),
                        GetCipherAlgorithmString(network.dot11DefaultCipherAlgorithm),
                        network.wlanSignalQuality,
                    };

                    listWiFiInfo.push_back(info);
                }

                // Release the network list memory of this interface
                if (pBssList != NULL)
                {
                    WlanFreeMemory(pBssList);
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

    ip_info_t GetNetworkIPByName(std::string targetAdapterName)
    {
        ip_info_t info;

        if (targetAdapterName.empty())
        {
            return info;
        }

        ULONG tableSize = 0;
        GetIpAddrTable(NULL, &tableSize, FALSE);

        PMIB_IPADDRTABLE pIPAddrTable = (PMIB_IPADDRTABLE)malloc(tableSize);
        if (GetIpAddrTable(pIPAddrTable, &tableSize, FALSE) != NO_ERROR)
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

                if (GetIfEntry(&ifRow) == NO_ERROR)
                {
                    std::string currentName((char*)ifRow.bDescr);

                    if (currentName.find(targetAdapterName) != std::string::npos)
                    {
                        // Buffers to store IP strings safely (INET_ADDRSTRLEN = 16 bytes for IPv4)
                        char ipBuffer[INET_ADDRSTRLEN] = { 0 };
                        char maskBuffer[INET_ADDRSTRLEN] = { 0 };

                        struct in_addr ipAddr = {}, maskAddr = {};
                        ipAddr.S_un.S_addr = pIPAddrTable->table[i].dwAddr;
                        maskAddr.S_un.S_addr = pIPAddrTable->table[i].dwMask;

                        // Modern and thread-safe conversion using inet_ntop
                        if (inet_ntop(AF_INET, &ipAddr, ipBuffer, sizeof(ipBuffer)) != nullptr)
                        {
                            info.ip = ipBuffer;
                        }

                        if (inet_ntop(AF_INET, &maskAddr, maskBuffer, sizeof(maskBuffer)) != nullptr)
                        {
                            info.mask = maskBuffer;
                        }

                        // Query Route Table for Gateway
                        ULONG routeSize = 0;
                        GetIpForwardTable(NULL, &routeSize, FALSE);
                        PMIB_IPFORWARDTABLE pRouteTable = (PMIB_IPFORWARDTABLE)malloc(routeSize);

                        if (pRouteTable)
                        {
                            if (GetIpForwardTable(pRouteTable, &routeSize, FALSE) == NO_ERROR)
                            {
                                for (DWORD j = 0; j < pRouteTable->dwNumEntries; j++)
                                {
                                    if ((pRouteTable->table[j].dwForwardIfIndex == ifRow.dwIndex)
                                        && (pRouteTable->table[j].dwForwardDest == 0))
                                    {
                                        char gwBuffer[INET_ADDRSTRLEN] = { 0 };
                                        struct in_addr gwAddr = {};
                                        gwAddr.S_un.S_addr = pRouteTable->table[j].dwForwardNextHop;

                                        if (inet_ntop(AF_INET, &gwAddr, gwBuffer, sizeof(gwBuffer)) != nullptr) {
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

    std::vector<std::string> CreateNetworkTypeDropdownOptions()
    {
        std::vector<std::string> result;

        for (const auto& info : listLanInfo)
        {
            result.push_back(info.name);
        }

        return result;
    }

    std::vector<std::string> CreateSSIDDropdownOptions()
    {
        std::vector<std::string> result;

        for (const auto& info : listWiFiInfo)
        {
            result.push_back(info.ssid);
        }

        return result;
    }

    bool ConnectWiFiWithTemplate(const wifi_info_t& info)
    {
        if (info.IsAnyNone())
        {
            return false;
        }

        // Reads the entire contents of an XML template file into a std::string
        std::ifstream templateFile(common_lib::ConvertWStringToString(common_lib::GetFullPath(L"wifi_template.xml")));
        if (!templateFile.is_open())
        {
            return false;
        }

        std::stringstream buffer;
        buffer << templateFile.rdbuf();
        std::string xmlTemplate = buffer.str();
        templateFile.close();

        // Use snprintf to format the string safely
        // Calculate the required buffer size (template length + length of the data passed in)
        size_t targetSize = xmlTemplate.size() + (info.ssid.size() * 2) + info.password.size() + info.auth.size() + info.cipher.size() + 1;
        std::vector<char> formattedXml(targetSize);

        // Match data to the %s positions in the template
        int result = snprintf(formattedXml.data(), targetSize, xmlTemplate.c_str(),
            info.ssid.c_str(), info.ssid.c_str(), info.auth.c_str(), info.cipher.c_str(), info.password.c_str());

        if (result < 0 || result >= targetSize)
        {
            return false;
        }

        // Save it as a temporary XML file containing the information
        std::string tempFileName = common_lib::ConvertWStringToString(common_lib::GetFullPath(L"wifi_ready.xml"));
        std::ofstream outFile(tempFileName);
        if (!outFile.is_open())
        {
            return false;
        }

        outFile << formattedXml.data();
        outFile.close();

        // Add profile to system
        std::string addCmd = "/c netsh wlan add profile filename=\"" + tempFileName + "\" >nul";
        // Issue command to connect to SSID
        std::string connCmd = "/c netsh wlan connect name=\"" + info.ssid + "\" ssid=\"" + info.ssid + "\"";

        //std::system(addCmd.c_str());
        //std::system(connCmd.c_str());
        ::ShellExecute(NULL, L"open", L"cmd", common_lib::ConvertStringToWString(addCmd).c_str(), common_lib::GetSystemPath().c_str(), SW_HIDE);
        ::Sleep(TIMECYCLE_100MS);
        ::ShellExecute(NULL, L"open", L"cmd", common_lib::ConvertStringToWString(connCmd).c_str(), common_lib::GetSystemPath().c_str(), SW_HIDE);
        ::Sleep(TIMECYCLE_100MS);

        // Delete the temporary .xml file to protect password information
        std::string deleteCmd = "del " + tempFileName;
        std::system(deleteCmd.c_str());

        return true;
    }

    bool IsWiFiConnectedTo(std::string targetSsid)
    {
        std::string output = common_lib::ExecuteCMD("netsh wlan show interfaces");

        if (targetSsid == "")
        {
            if ((output.find("connected") != std::string::npos)
                && (output.find("disconnected") == std::string::npos))
            {
                return true;
            }
        }
        else
        {
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
}
