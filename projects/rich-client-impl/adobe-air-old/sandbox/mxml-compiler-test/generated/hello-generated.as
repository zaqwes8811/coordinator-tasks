
/**
 *  Generated by mxmlc 4.0
 *
 *  Package:    
 *  Class:      hello
 *  Source:     K:\planner-katya\sandbox\hello.mxml
 *  Template:   flex2/compiler/mxml/gen/ClassDef.vm
 *  Time:       2012.03.09 21:13:57 MSK
 */

package 
{

import flash.accessibility.*;
import flash.debugger.*;
import flash.display.*;
import flash.errors.*;
import flash.events.*;
import flash.events.MouseEvent;
import flash.external.*;
import flash.filters.*;
import flash.geom.*;
import flash.media.*;
import flash.net.*;
import flash.printing.*;
import flash.profiler.*;
import flash.system.*;
import flash.text.*;
import flash.ui.*;
import flash.utils.*;
import flash.xml.*;
import mx.binding.*;
import mx.containers.VBox;
import mx.controls.Button;
import mx.controls.TextInput;
import mx.core.Application;
import mx.core.ClassFactory;
import mx.core.DeferredInstanceFromClass;
import mx.core.DeferredInstanceFromFunction;
import mx.core.IDeferredInstance;
import mx.core.IFactory;
import mx.core.IFlexModuleFactory;
import mx.core.IPropertyChangeNotifier;
import mx.core.UIComponentDescriptor;
import mx.core.mx_internal;
import mx.styles.*;


[Frame(extraClass="_hello_FlexInit")]

[Frame(factoryClass="_hello_mx_managers_SystemManager")]


public class hello
    extends mx.core.Application
{

    [Bindable]
	/**
 * @private
 **/
    public var myText : mx.controls.TextInput;

    [Bindable]
	/**
 * @private
 **/
    public var mybutton : mx.controls.Button;




private var _documentDescriptor_ : mx.core.UIComponentDescriptor = 
new mx.core.UIComponentDescriptor({
  type: mx.core.Application
  ,
  propertiesFactory: function():Object { return {
    childDescriptors: [
      new mx.core.UIComponentDescriptor({
        type: mx.containers.VBox
        ,
        propertiesFactory: function():Object { return {
          childDescriptors: [
            new mx.core.UIComponentDescriptor({
              type: mx.controls.TextInput
              ,
              id: "myText"
              ,
              propertiesFactory: function():Object { return {
                text: "Hello World!"
              }}
            })
          ,
            new mx.core.UIComponentDescriptor({
              type: mx.controls.Button
              ,
              id: "mybutton"
              ,
              events: {
                click: "__mybutton_click"
              }
              ,
              propertiesFactory: function():Object { return {
                label: "Get Weather"
              }}
            })
          ]
        }}
      })
    ]
  }}
})

    /**
     * @private
     **/
    public function hello()
    {
        super();

        mx_internal::_document = this;




        // layer initializers

       
        // properties


        // events




    }

    /**
     * @private
     **/ 
    private var __moduleFactoryInitialized:Boolean = false;

    /**
     * @private
     * Override the module factory so we can defer setting style declarations
     * until a module factory is set. Without the correct module factory set
     * the style declaration will end up in the wrong style manager.
     **/ 
    override public function set moduleFactory(factory:IFlexModuleFactory):void
    {
        super.moduleFactory = factory;
        
        if (__moduleFactoryInitialized)
            return;

        __moduleFactoryInitialized = true;


        // our style settings


        // ambient styles
        mx_internal::_hello_StylesInit();

                         
    }
 
    /**
     * @private
     **/
    override public function initialize():void
    {
        mx_internal::setDocumentDescriptor(_documentDescriptor_);


        super.initialize();
    }



            private function writeToLog():void          {
                trace(myText.text);
            }
        



    //  supporting function definitions for properties, events, styles, effects
/**
 * @private
 **/
public function __mybutton_click(event:flash.events.MouseEvent):void
{
	writeToLog();
}




    mx_internal var _hello_StylesInit_done:Boolean = false;

    mx_internal function _hello_StylesInit():void
    {
        //  only add our style defs to the style manager once
        if (mx_internal::_hello_StylesInit_done)
            return;
        else
            mx_internal::_hello_StylesInit_done = true;
            
        var style:CSSStyleDeclaration;
        var effects:Array;
                    

        var conditions:Array;
        var condition:CSSCondition;
        var selector:CSSSelector;

        styleManager.initProtoChainRoots();
    }




}

}