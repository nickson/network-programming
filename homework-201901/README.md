## C/C++

#### Зад 1.
Да се преработи кодът от файловете:
[server DNS](https://github.com/nickson/network-programming/blob/master/TCP_sockets/TCP_DNSserver_IPv6_AAAA.c) 
[client DNS](https://github.com/nickson/network-programming/blob/master/TCP_sockets/TCP_DNSclient_IPv6.c)
така че сървърът да връща към клиента (вместо само 1вия намерен ipv6 адрес) всички ipv4 и ipv6 адреси, които съответстват на домейн името, подадено от клиента.
Напр. Клиентът се изпълнява със следните параметри:
```
\# ./TCP_DNSclient_IPv6_AAAA.out <server_IPv6_addesss> yahoo.com
A total of 6 IPv4 addressses resolved:
PF_INET resolved address: 98.138.219.232
PF_INET resolved address: 98.137.246.8
PF_INET resolved address: 98.138.219.231
PF_INET resolved address: 72.30.35.9
PF_INET resolved address: 98.137.246.7
PF_INET resolved address: 72.30.35.10
A total of 6 IPv6 addressses resolved:
AP_INET6 resolved address: 2001:4998:c:1023::5
AP_INET6 resolved address: 2001:4998:58:1836::10
AP_INET6 resolved address: 2001:4998:44:41d::4
AP_INET6 resolved address: 2001:4998:44:41d::3
AP_INET6 resolved address: 2001:4998:58:1836::11
AP_INET6 resolved address: 2001:4998:c:1023::4

\# ./TCP_DNSclient_IPv6_AAAA.out <server_IPv6_addesss> abv.bg
A total of 1 IPv4 addressses resolved:
PF_INET resolved address: 194.153.145.104
No IPv6 addresses resolved.

\# ./TCP_DNSclient_IPv6_AAAA.out <server_IPv6_addesss> uni-sofia.bg
A total of 1 IPv4 addressses resolved:
PF_INET resolved address: 62.44.96.22
A total of 1 IPv6 addressses resolved:
AP_INET6 resolved address: 2001:67c:20d0::22
```
*Hint:*  Можете да ползвате hints.ai_family=AF_UNSPEC, за да оплучите едновременно IPv4 и IPv6.


#### Зад 2.
Да се преработи кодът от файлa:
https://github.com/nickson/network-programming/blob/master/IO_Multiplexing-Synchromous/select_chatServer.c 
При свързване през telnet или написан от Вас клиент, първоначално да се пита за потребителско име, сървърът да запомня какво потребителско име е подадено от съответния мрежови сокет (в структура или във файл). При всяко следващо съобщение от клиента, да се изпиства в началото потребителското име на изпращащия
<username> says: <message>

Пример:
telnet <server_IP> 3333
Enter your username:
user1
hello everybody!

Останалите, свързали се  по същия начин получават:
user1 says: hello everybody!

Когато потребител прекрати връзката, изнформацията за него да се изтрива, за да може друг да ползва същия файлов дескриптор на сокет с различно поребителско име.


Java

Зад. 3:
Да се преработи кодът от:
https://github.com/nickson/network-programming/tree/master/Java-TCP 
Вместо сървърът да отваря нова нишка (thread) за всеки клиент, да създава thread pool, от който да се земат временно нишки със синхронизация.

Зад. 4:
Да се преработи кодът от:
https://github.com/nickson/network-programming/tree/master/Java-TCP 
Комуникацияра да става по TLS/SSL, като сървърът се аутентикира с X.509 сертификат.