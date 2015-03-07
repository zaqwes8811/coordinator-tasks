

import flash.events.Event;
import flash.events.EventDispatcher;
import flash.events.IEventDispatcher;
import mx.core.IPropertyChangeNotifier;
import mx.events.PropertyChangeEvent;
import mx.utils.ObjectProxy;
import mx.utils.UIDUtil;

import mx.controls.Button;
import mx.controls.TextInput;

class BindableProperty
{
	/*
	 * generated bindable wrapper for property myText (public)
	 * - generated setter
	 * - generated getter
	 * - original public var 'myText' moved to '_1060040455myText'
	 */

    [Bindable(event="propertyChange")]
    public function get myText():mx.controls.TextInput
    {
        return this._1060040455myText;
    }

    public function set myText(value:mx.controls.TextInput):void
    {
    	var oldValue:Object = this._1060040455myText;
        if (oldValue !== value)
        {
            this._1060040455myText = value;
           if (this.hasEventListener("propertyChange"))
               this.dispatchEvent(mx.events.PropertyChangeEvent.createUpdateEvent(this, "myText", oldValue, value));
        }
    }

	/*
	 * generated bindable wrapper for property mybutton (public)
	 * - generated setter
	 * - generated getter
	 * - original public var 'mybutton' moved to '_376159266mybutton'
	 */

    [Bindable(event="propertyChange")]
    public function get mybutton():mx.controls.Button
    {
        return this._376159266mybutton;
    }

    public function set mybutton(value:mx.controls.Button):void
    {
    	var oldValue:Object = this._376159266mybutton;
        if (oldValue !== value)
        {
            this._376159266mybutton = value;
           if (this.hasEventListener("propertyChange"))
               this.dispatchEvent(mx.events.PropertyChangeEvent.createUpdateEvent(this, "mybutton", oldValue, value));
        }
    }



}
