// KnobArray demo
// Potentiometer version [3x2]

KnobArray knobArray;

void setup(){
  knobArray = new KnobArray(this, "COM14");
}

void draw(){
  for (int i = 0; i < 6; i++){
    print(knobArray.values[i]);
    print('\t');
  }
  println();
}

void serialEvent(Serial port) {
  if (port == knobArray.serial){ //<>//
    knobArray.serialEvent();
  }
}
