Hi, download OpenSSL, then run the file openssl.exe that is located in "bin" folder. You have to see the command line shell. 
Enter the follow command:
OpenSSL> s_client -showcerts -verify 5 -connect yourHiveMqClusterName.s1.eu.hivemq.cloud:8883

Don't forget to replace the "yourHiveMqClusterName" field with your cluster name


Generate new ssl certificate as per https://www.youtube.com/watch?v=WGx06lPTNyQ 