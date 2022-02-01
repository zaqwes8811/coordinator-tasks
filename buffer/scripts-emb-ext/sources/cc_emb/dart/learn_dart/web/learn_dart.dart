//import 'dart:html';

class InputField {
  //public:
  final int maxLength = 10;
  String name;
}

void main() {
  //querySelector("#sample_text_id")
  //  ..text = "Click me!"
  //  ..onClick.listen(reverseText);
  
  var tmp = new InputField();
  var nvar = tmp.maxLength;
  print("var = $nvar");
  
  
}

/*
void reverseText(MouseEvent event) {
  var text = querySelector("#sample_text_id").text;
  var buffer = new StringBuffer();
  for (int i = text.length - 1; i >= 0; i--) {
    buffer.write(text[i]);
  }
  querySelector("#sample_text_id").text = buffer.toString();
}*/
