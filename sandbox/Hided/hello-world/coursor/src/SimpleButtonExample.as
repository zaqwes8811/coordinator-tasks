package {
    import flash.display.Sprite;
    
    public class SimpleButtonExample extends Sprite {
		private var csb_:CustomSimpleButton;
		//private var xp:uint;
		//private var yp:uint;
        public function SimpleButtonExample() {
            csb_ = new CustomSimpleButton();
			this.csb_.y = 40;
            addChild(csb_);
        }
		///
		public function setPos(x:uint, y:uint) : void {
			csb_.x = x; // center? how do it?
			csb_.y = y;
		}
    }
}

import flash.display.DisplayObject;
import flash.display.Shape;
import flash.display.SimpleButton;

class CustomSimpleButton extends SimpleButton {
    private var upColor_:uint   = 0xFFCC00;
    private var overColor_:uint = 0xCCFF00;
    private var downColor_:uint = 0x00CCFF;
    private var size_:uint      = 40;

    public function CustomSimpleButton() {
        downState = new ButtonDisplayState(downColor_, size_);
        overState = new ButtonDisplayState(overColor_, size_);
        upState = new ButtonDisplayState(upColor_, size_);
        hitTestState = new ButtonDisplayState(upColor_, size_ * 2);
        hitTestState.x = -(size_ / 4);
        hitTestState.y = hitTestState.x;
        useHandCursor  = true;
    }
}

/// view
class ButtonDisplayState extends Shape {
    private var bgColor_:uint;
    private var size_:uint;

    public function ButtonDisplayState(bgColor:uint, size:uint) {
        this.bgColor_ = bgColor;
        this.size_    = size;
        draw();
    }

    private function draw():void {
        graphics.beginFill(bgColor_);
        graphics.drawRect(0, 0, size_, size_);
        graphics.endFill();
    }
}
