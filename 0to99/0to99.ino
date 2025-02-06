int tens[] = {34, 35, 32, 33, 25, 26, 27}, ones[] = {14, 12, 13, 2, 4, 5, 18}, count = 0;

void setup() {
  for(int i = 0; i<7; i++) {
    pinMode(ones[i], OUTPUT);
    pinMode(tens[i], OUTPUT);
  }
}

void loop() {
  displaynum(tens, count/10);
  displaynum(ones, count%10);
  count++;
  if(count>99) count = 0;
  delay(1000);
}

void displaynum(int array[], int num) {
  clear(array);
  switch (num) {
    case 0: //0,1,2,3,4,5
            digitalWrite(array[0], LOW);
            digitalWrite(array[1], LOW);
            digitalWrite(array[2], LOW);
            digitalWrite(array[3], LOW);
            digitalWrite(array[4], LOW);
            digitalWrite(array[5], LOW);
            break;
    case 1: //1,2
            digitalWrite(array[1], LOW);
            digitalWrite(array[2], LOW);
            break;
    case 2: //0,1,3,4,6
            digitalWrite(array[0], LOW);
            digitalWrite(array[1], LOW);
            digitalWrite(array[3], LOW);
            digitalWrite(array[4], LOW);
            digitalWrite(array[6], LOW);
            break;
    case 3: //0,1,2,3,6
            digitalWrite(array[0], LOW);
            digitalWrite(array[1], LOW);
            digitalWrite(array[2], LOW);
            digitalWrite(array[3], LOW);
            digitalWrite(array[6], LOW);
            break;
    case 4: //1,2,5,6
            digitalWrite(array[1], LOW);
            digitalWrite(array[2], LOW);
            digitalWrite(array[5], LOW);
            digitalWrite(array[6], LOW);
            break;
    case 5: //0,2,3,5.6
            digitalWrite(array[0], LOW);
            digitalWrite(array[2], LOW);
            digitalWrite(array[3], LOW);
            digitalWrite(array[5], LOW);
            digitalWrite(array[6], LOW);
            break;
    case 6: //0,2,3,4,5,6
            digitalWrite(array[0], LOW);
            digitalWrite(array[2], LOW);
            digitalWrite(array[3], LOW);
            digitalWrite(array[4], LOW);
            digitalWrite(array[5], LOW);
            digitalWrite(array[6], LOW);
            break;
    case 7: //0,1,2
            digitalWrite(array[0], LOW);
            digitalWrite(array[1], LOW);
            digitalWrite(array[2], LOW);
            break;
    case 8: //0,1,2,3,4,5,6
            digitalWrite(array[0], LOW);
            digitalWrite(array[1], LOW);
            digitalWrite(array[2], LOW);
            digitalWrite(array[3], LOW);
            digitalWrite(array[4], LOW);
            digitalWrite(array[5], LOW);
            digitalWrite(array[6], LOW);
            break;
    case 9: //0,1,2,3,5,6
            digitalWrite(array[0], LOW);
            digitalWrite(array[1], LOW);
            digitalWrite(array[2], LOW);
            digitalWrite(array[3], LOW);
            digitalWrite(array[5], LOW);
            digitalWrite(array[6], LOW);
            break;
  }
}

void clear(int array[]) {
  for(int i = 0; i<7; i++)
    digitalWrite(array[i], HIGH);
}