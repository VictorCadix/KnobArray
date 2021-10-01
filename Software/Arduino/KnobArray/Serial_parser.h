#pragma once

#define BUFFER_SIZE 16
#define MSG_SIZE 13

class Serial_parser {

    bool RxStart = false;
  	bool RxEnd = true;
  	unsigned int RxCont = 0;
  	char RxBuff[BUFFER_SIZE];
	char msg_label[3];
	char msg[MSG_SIZE];

public:
	HardwareSerial *serialPort;
	HardwareSerial *serialPort_debug;

    int dataStream;

    int serialPort_read();
    int parseInputData();
};

int indexOf(char c, char* buff){
	for (int i=0; buff[i]!='\0'; i++){
		if (c == buff[i]){
			return i;
		}
	}
	return -1;
}

int Serial_parser::serialPort_read(){
	while (serialPort->available()) {
		char token = serialPort->read();
		//serialPort_debug->print(token);

		if (token == '#') {
      		RxStart = true;
			RxEnd = false;
      		RxCont = 0;
    	}
		else if (token == ';') {
			RxBuff[RxCont] = '\0';
      		RxStart = false;
			RxEnd = true;
      		RxCont++;
			//serialPort_debug->println();
			return true;
    	}
		else if (RxStart == true) {
        	RxBuff[RxCont] = token;
        	if(RxCont < BUFFER_SIZE){     //avoiding buffer overflow
          		RxCont++;
        	}
      	}
	}
	return false;
}

int Serial_parser::parseInputData() {
	bool msg_in = serialPort_read();
	if (msg_in){
		int start_msg_pos = indexOf(':', RxBuff) + 1;
		
		//Gets the body of the message
		if (start_msg_pos > 0){
			for (int i = 0; i < MSG_SIZE; i++){
				msg[i] = RxBuff[start_msg_pos + i];
				if (msg[i] == '\0'){
					break;
				}
			}
		}
		else {
			serialPort->print("#':' not found\r\n;");
			return 0;
		}
		//Gets message label
		for (int i = 0; i < start_msg_pos-1; i++){
			msg_label[i] = RxBuff[i];
			if (i+1 == start_msg_pos-1){
				msg_label[i+1] = '\0';
			}
		}
		
		//Label check
		if (0 == strcmp(msg_label, "DS")){	
			//XY position
			dataStream = strtol(msg, NULL, 10);
    		return 1;
		}
		else if (0 == strcmp(msg_label, "R")){
			return -1;	//Reset
		}
		else {
			serialPort->println("#Unknown comand");
		}
	}
    return 0;
}
