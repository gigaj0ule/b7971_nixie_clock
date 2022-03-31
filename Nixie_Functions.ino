void NIXIE_WRITE_broadcast_begin(){
  digitalWriteFast(PIN_digit_one,   LOW);
  digitalWriteFast(PIN_digit_two,   LOW);
  digitalWriteFast(PIN_digit_three, LOW);
  digitalWriteFast(PIN_digit_four,  LOW);
};


void NIXIE_WRITE_broadcast_end(){
  digitalWriteFast(PIN_digit_one,   HIGH);
  digitalWriteFast(PIN_digit_two,   HIGH);
  digitalWriteFast(PIN_digit_three, HIGH);
  digitalWriteFast(PIN_digit_four,  HIGH);
};


void NIXIE_WRITE_blank( int milli_s ){
  NIXIE_WRITE_broadcast_begin();
  mcp.digitalWrite(ascii[' ']);
  NIXIE_WRITE_broadcast_end();
  if(milli_s) delay(milli_s);
};


void NIXIE_WRITE_letter(int letter, byte character, byte has_underscore){
  byte cs_pin = 0;
  
  switch(letter){
    case 1:
     cs_pin = PIN_digit_one;
     break;
    case 2:
     cs_pin = PIN_digit_two;
     break;
    case 3:
     cs_pin = PIN_digit_three;
     break;
    case 4:
     cs_pin = PIN_digit_four;
     break;
    default:
     return;
  };
  
  int write_byte;
  
  if(has_underscore){
    write_byte = ascii[character] | 0b0100000000000000;
  }else{
    write_byte = ascii[character];
  }

  digitalWriteFast(cs_pin, LOW);
  mcp.digitalWrite(write_byte);
  digitalWriteFast(cs_pin, HIGH);
};


void NIXIE_WRITE_scroll_message(String message, int speed_ms){
  String window;
  String padding;
  
  padding = "    ";
  padding.concat(message);
  message = padding;
  message.concat("       ");
  
  int x = 0;
  while(x < int(message.length()) - 4 ){
    window = message.substring(x);
    NIXIE_WRITE_letter(1, window[0], 0);
    NIXIE_WRITE_letter(2, window[1], 0);
    NIXIE_WRITE_letter(3, window[2], 0);
    NIXIE_WRITE_letter(4, window[3], 0);
    Serial.println(window);
    delay(speed_ms);
    x = x + 1;
  };
};


void ANIMATION_init_test(){
  int i;
  for(i = 0; i < 14; i++){
    NIXIE_WRITE_broadcast_begin();
    mcp.digitalWrite(ascii[i]);
    NIXIE_WRITE_broadcast_end();
    delay(50);
  };
  
  ANIMATION_flash(5);
};


void ANIMATION_flash(int flashed){
  byte i = 0;
  while(i < flashed + 1){
    NIXIE_WRITE_broadcast_begin();
    mcp.digitalWrite(ascii[14]);
    NIXIE_WRITE_broadcast_end();
    delay(50);
    NIXIE_WRITE_blank(20);
    i ++;
  }; 
};


void make_font_table(){
             //0Bponmlkjihgfedcba
  ascii['A'] = 0b0000000011101111;
  ascii['B'] = 0b0000100111110001;
  ascii['C'] = 0b0000000010011100;
  ascii['D'] = 0b0000100111110000;
  ascii['E'] = 0b0000000010011110;
  ascii['F'] = 0b0000000010001111;
  ascii['G'] = 0b0000000010111101;
  ascii['H'] = 0b0000000001101111;
  ascii['I'] = 0b0000100110010000;
  ascii['J'] = 0b0000000001111000;
  ascii['K'] = 0b0000011000001110;
  ascii['L'] = 0b0000000000011100;
  ascii['M'] = 0b0010001001101100;
  ascii['N'] = 0b0010010001101100;
  ascii['O'] = 0b0000000011111100;
  ascii['P'] = 0b0000000011001111;
  ascii['Q'] = 0b0000010011111100;
  ascii['R'] = 0b0000010011001111;
  ascii['S'] = 0b0000000010110111;
  ascii['T'] = 0b0000100110000000;
  ascii['U'] = 0b0000000001111100;
  ascii['V'] = 0b0001001000001100;
  ascii['W'] = 0b0001010001101100;
  ascii['X'] = 0b0011011000000000;
  ascii['Y'] = 0b0010101000000000;
  ascii['Z'] = 0b0001001010010000;
             //0Bponmlkjihgfedcba

             //0Bponmlkjihgfedcba
  ascii['a'] = 0b0001000011110001;
  ascii['b'] = 0b0000010000011110;
  ascii['c'] = 0b0000000000011010;
  ascii['d'] = 0b0001000001110001;
  //ascii['e'] = 0b0000000010011110;
  ascii['e'] = 0b0001000000011010;
  ascii['f'] = 0b0000000010001110;
  ascii['g'] = 0b0010000011110001;
  ascii['h'] = 0b0000000000101111;
  ascii['i'] = 0b0000100000000000;
  ascii['j'] = 0b0001000100001000;
  ascii['k'] = 0b0000111100000000;
  ascii['l'] = 0b0000100100000000;
  ascii['m'] = 0b0000100000101011;
  ascii['n'] = 0b0000010000001010;
  ascii['o'] = 0b0000000000111011;
  ascii['p'] = 0b0000001010001110;
  ascii['q'] = 0b0000010011000111;
  ascii['r'] = 0b0000000000001010;
  ascii['s'] = 0b0010000010110001;
  ascii['t'] = 0b0000000000011110;
  ascii['u'] = 0b0000000000111000;
  ascii['v'] = 0b0001000000001000;
  ascii['w'] = 0b0001010000101000;
  ascii['x'] = 0b0011011000000000;
  ascii['y'] = 0b0010000001110001;  
  ascii['z'] = 0b0001000000010010;
             //0Bponmlkjihgfedcba

             //0Bponmlkjihgfedcba
  ascii[':'] = 0b0000000010010000;
  ascii['_'] = 0b0100000000000000;
  ascii[' '] = 0b0000000000000000;
  ascii['|'] = 0b0000100100000000;
  ascii['!'] = 0b0100100100000000;
  ascii['?'] = 0b0100100011000101;
  ascii['*'] = 0b0011111100000011;
  ascii['/'] = 0b0001001000000000;
  ascii['+'] = 0b0000100100000011;
  ascii['-'] = 0b0000000000000011;
  ascii['#'] = 0b0000100111111111;
  ascii['>'] = 0b0011000000000000;
  ascii['<'] = 0b0000011000000000;
  ascii['['] = 0b0000000010011100;
  ascii[']'] = 0b0000000011110000;
  ascii['$'] = 0b0000100110110111;
  ascii['\'']= 0b0000000100000000;
  ascii['"'] = 0b0000000101000000;
             //0Bponmlkjihgfedcba

             //0Bponmlkjihgfedcba
  ascii['0'] = 0b0000000011111100;
  ascii['1'] = 0b0000000001100000;
  ascii['2'] = 0b0001000011010001;
  ascii['3'] = 0b0000000011110011;
  ascii['4'] = 0b0000000001100111;
  ascii['5'] = 0b0000010010010110;
  ascii['6'] = 0b0000000010111111;
  ascii['7'] = 0b0000101010000000;
  ascii['8'] = 0b0000000011111111;
  ascii['9'] = 0b0000000011100111;
             //0Bponmlkjihgfedcba
             
            //0Bponmlkjihgfedcba
  ascii[0]  = 0b0000000000000001;
  ascii[1]  = 0b0000000000000010;
  ascii[2]  = 0b0000000000000100;
  ascii[3]  = 0b0000000000001000;
  ascii[4]  = 0b0000000000010000;
  ascii[5]  = 0b0000000000100000;
  ascii[6]  = 0b0000000001000000;
  ascii[7]  = 0b0000000010000000;
  ascii[8]  = 0b0000000100000000;
  ascii[9]  = 0b0000001000000000;
  ascii[10] = 0b0000010000000000;
  ascii[11] = 0b0000100000000000;
  ascii[12] = 0b0001000000000000;
  ascii[13] = 0b0010000000000000;
  ascii[14] = 0b0011111111111111;
            //0Bponmlkjihgfedcba
};
