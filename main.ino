//Terceira versão

#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>


byte mac[]    = {  0xDE, 0xED, 0xBA, 0xFE, 0xFE, 0xED };
//IPAddress ip(172, 16, 0, 100); // endereço do gateway
//IPAddress server(172, 16, 1, 62); //endereço do computador
char server[] = "m14.cloudmqtt.com"; 
//Porta ligada ao pino IN1 do modulo
int porta_rele1 = 7;

void callback(char* topic, byte* payload, unsigned int length);


EthernetClient ethClient;
PubSubClient client(server,1883,callback,ethClient);

void setup()
{
  Serial.begin(115200);
  //Define pinos para o rele como saida
  pinMode(porta_rele1, OUTPUT);
  digitalWrite(porta_rele1, HIGH); //Desliga rele 1
  
  client.setServer(server,17676);
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
  if (msg.equals("1")){
    //aqui coloca comando para ligar o relé
      digitalWrite(porta_rele1, LOW);
      Serial.println("LIGADO");
      client.publish("Cafe","LIGADO");
      
    }

   if (msg.equals("0")){
    //aqui coloca o comando pra desligar
     digitalWrite(porta_rele1, HIGH); //Desliga rele 1
     Serial.println("DESLIGADO");
     client.publish("Cafe","DESLIGADO");
    }  
  
}

void loop()
{

  if(!client.connected()){
    client.connect("arduinoClient","qiahkics","mxmKGjKQtj4X");
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("Cafe","Start");
      // ... and resubscribe
      client.subscribe("Cafe");
    
    }
  client.loop();
}
