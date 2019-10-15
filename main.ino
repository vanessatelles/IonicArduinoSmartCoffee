//Segunda versão

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>


byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
//IPAddress ip(192, 168, 0, 27); // endereço do gateway
//IPAddress server(192, 168, 0, 16); //endereço do computador, ipv4
//char server[] = "broker.hivemq.com"; 
//Porta ligada ao pino IN1 do modulo
int porta_rele1 = 7;

void callback(char* topic, byte* payload, unsigned int length);


EthernetClient ethClient;
PubSubClient client(server,1883,callback,ethClient);

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("batatapub","Start");
      // ... and resubscribe
      client.subscribe("batatasub");

     
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(115200);
  //Define pinos para o rele como saida
  pinMode(porta_rele1, OUTPUT);
  
  client.setServer(server, 1883);
  client.setCallback(callback);  

  // Ethernet.begin(mac,ip); // Se falhar configura o IP para um ip vazio na rede!
 // Inicia a Conexão Ethernet em modo DHCP:
     if (Ethernet.begin(mac) == 0) {
      
     Serial.println("Failed to configure Ethernet using DHCP");

    }

}

void callback(char* topic, byte* payload, unsigned int length) {
String msg; //String que vai receber a mensagem do tópico
  
  for (int i=0;i<length;i++) {
    char c= (char)payload[i];
    msg+= c;
    Serial.print((char)payload[i]);
  }
  Serial.println();

  //dependendo do conteúdo da mensagem realiza uma ação
  if (msg.equals("L")){
    //aqui coloca comando para ligar o relé
      digitalWrite(porta_rele1, LOW);
      Serial.println("LIGADO");
      client.publish("batatapub","LIGADO");
      
    }

   if (msg.equals("D")){
    //aqui coloca o comando pra desligar
     digitalWrite(porta_rele1, HIGH); //Desliga rele 1
     Serial.println("DESLIGADO");
     client.publish("batatapub","DESLIGADO");
    }  
  
}

void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  
  client.loop();
}
