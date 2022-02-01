/** 
  file : UiBoard.qml
  
  Abstract : ���-������ ��� ���������� ��������
  
  Autor : Igor Lugansky
  
  Mod. history :
    26-oct IL create file
  
*/
import QtQuick 1.0
Rectangle {
  id : rectTop

  /// prop
  property int quad : 300
 
  /// obj. prop.
  width : quad; height : quad
  
  /// Forge ///
  property int gWi : 200  // ����� ������ ���� ��������
  property double dRatio : 0.4
  UiParseTop {
    id : pt
    anchors.left : ulabels.right
    wi : gWi*dRatio
  }
  UiLabels {
    id : ulabels
    wi : gWi*(1-dRatio)
  }
  
  ///
}