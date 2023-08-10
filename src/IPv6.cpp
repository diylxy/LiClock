#include "A_Config.h"
#include "lwip/dns.h"
esp_ip6_addr_t ipv6global;
esp_ip6_addr_t ipv6local;
esp_netif_t *get_esp_interface_netif(esp_interface_t interface);
void enableIPv6()
{
    ip_addr_t dnsip;
    dnsip.type = IPADDR_TYPE_V4;
    ipaddr_aton("223.5.5.5", &dnsip);
    dns_setserver(1, &dnsip);
    ip6addr_aton("240C::6666", &(dnsip.u_addr.ip6));
    dnsip.type = IPADDR_TYPE_V6;
    dns_setserver(2, &dnsip);
    WiFi.enableIpV6();
    esp_netif_get_ip6_global(get_esp_interface_netif(ESP_IF_WIFI_STA), (esp_ip6_addr_t *)&ipv6global);
    esp_netif_get_ip6_linklocal(get_esp_interface_netif(ESP_IF_WIFI_STA), (esp_ip6_addr_t *)&ipv6local);
}
void refreshIPV6Addr()
{
    esp_netif_get_ip6_global(get_esp_interface_netif(ESP_IF_WIFI_STA), (esp_ip6_addr_t *)&ipv6global);
    esp_netif_get_ip6_linklocal(get_esp_interface_netif(ESP_IF_WIFI_STA), (esp_ip6_addr_t *)&ipv6local);
}
const char *ipv6_to_str(const esp_ip6_addr_t *addr)
{
    return ip6addr_ntoa((const ip6_addr_t *)addr);
}