package
{

import flash.display.LoaderInfo;
import flash.text.Font;
import flash.text.TextFormat;
import flash.text.engine.TextBlock;
import flash.text.engine.TextLine;
import flash.system.ApplicationDomain;
import flash.system.Security
import flash.utils.Dictionary;
import flash.utils.getDefinitionByName;
import flashx.textLayout.compose.ISWFContext;
import mx.core.IFlexModule;
import mx.core.IFlexModuleFactory;
import mx.core.RSLData;
import mx.events.RSLEvent;
import mx.core.FlexVersion;
import mx.managers.SystemManager;
import mx.preloaders.SparkDownloadProgressBar;

[ExcludeClass]
public class _hello_mx_managers_SystemManager
    extends mx.managers.SystemManager
    implements IFlexModuleFactory, ISWFContext
{
    // Cause the CrossDomainRSLItem class to be linked into this application.
    import mx.core.CrossDomainRSLItem; CrossDomainRSLItem;

    public function _hello_mx_managers_SystemManager()
    {

        super();
    }

    override     public function callInContext(fn:Function, thisArg:Object, argArray:Array, returns:Boolean=true):*
    {
        if (returns)
           return fn.apply(thisArg, argArray);
        else
           fn.apply(thisArg, argArray);
    }

    override     public function create(... params):Object
    {
        if (params.length > 0 && !(params[0] is String))
            return super.create.apply(this, params);

        var mainClassName:String = params.length == 0 ? "hello" : String(params[0]);
        var mainClass:Class = Class(getDefinitionByName(mainClassName));
        if (!mainClass)
            return null;

        var instance:Object = new mainClass();
        if (instance is IFlexModule)
            (IFlexModule(instance)).moduleFactory = this;
        return instance;
    }

    /**
     *  @private
     */
    private var _info:Object;

    override    public function info():Object
    {
        if (!_info)
        {
            _info = {
            cdRsls: [
[new RSLData("http://fpdownload.adobe.com/pub/swz/flex/4.6.0.23201/framework_4.6.0.23201.swz",
"http://fpdownload.adobe.com/pub/swz/crossdomain.xml",
"abd49354324081cebb8f60184cf5fee81f0f9298e64dbec968c96d68fe16c437",
"SHA-256",true,true,"default"),
new RSLData("framework_4.6.0.23201.swz",
"",
"abd49354324081cebb8f60184cf5fee81f0f9298e64dbec968c96d68fe16c437",
"SHA-256",true,true,"default")],
[new RSLData("http://fpdownload.adobe.com/pub/swz/tlf/2.0.0.232/textLayout_2.0.0.232.swz",
"http://fpdownload.adobe.com/pub/swz/crossdomain.xml",
"8f903698240fe799f61eeda8595181137b996156bb176da70ad6f41645c64c74",
"SHA-256",true,true,"default"),
new RSLData("textLayout_2.0.0.232.swz",
"",
"8f903698240fe799f61eeda8595181137b996156bb176da70ad6f41645c64c74",
"SHA-256",true,true,"default")],
[new RSLData("http://fpdownload.adobe.com/pub/swz/flex/4.6.0.23201/spark_4.6.0.23201.swz",
"http://fpdownload.adobe.com/pub/swz/crossdomain.xml",
"4bae91dbaef0ceec0fce5505d96ddea865edbfc14dd96e89fb54de12f58432eb",
"SHA-256",true,true,"default"),
new RSLData("spark_4.6.0.23201.swz",
"",
"4bae91dbaef0ceec0fce5505d96ddea865edbfc14dd96e89fb54de12f58432eb",
"SHA-256",true,true,"default")],
[new RSLData("http://fpdownload.adobe.com/pub/swz/flex/4.6.0.23201/sparkskins_4.6.0.23201.swz",
"http://fpdownload.adobe.com/pub/swz/crossdomain.xml",
"84d36bdf5e2577bfb0b8ce6a12a8646bb1aadddd24c92e5c12d157d44ad7ce61",
"SHA-256",true,true,"default"),
new RSLData("sparkskins_4.6.0.23201.swz",
"",
"84d36bdf5e2577bfb0b8ce6a12a8646bb1aadddd24c92e5c12d157d44ad7ce61",
"SHA-256",true,true,"default")],
[new RSLData("http://fpdownload.adobe.com/pub/swz/flex/4.6.0.23201/mx_4.6.0.23201.swz",
"http://fpdownload.adobe.com/pub/swz/crossdomain.xml",
"d888aee0ce49f58a35c32eb138edd00f0d6b9fae68b78d7eb83932c09ef0b6f1",
"SHA-256",true,true,"default"),
new RSLData("mx_4.6.0.23201.swz",
"",
"d888aee0ce49f58a35c32eb138edd00f0d6b9fae68b78d7eb83932c09ef0b6f1",
"SHA-256",true,true,"default")],
]
,
            compiledLocales: [ "en_US" ],
            compiledResourceBundleNames: [ "components", "containers", "controls", "core", "effects", "layout", "skins", "styles" ],
            currentDomain: ApplicationDomain.currentDomain,
            mainClassName: "hello",
            mixins: [ "_hello_FlexInit", "_hello_Styles" ],
            placeholderRsls: [
[new RSLData("http://fpdownload.adobe.com/pub/swz/flex/4.6.0.23201/rpc_4.6.0.23201.swz",
"http://fpdownload.adobe.com/pub/swz/crossdomain.xml",
"98eeca3e014a0fa3c4c613006bdcea12da3beace6a8c9eccdde2e07cb486bab5",
"SHA-256",true,true,"default"),
new RSLData("rpc_4.6.0.23201.swz",
"",
"98eeca3e014a0fa3c4c613006bdcea12da3beace6a8c9eccdde2e07cb486bab5",
"SHA-256",true,true,"default")],
[new RSLData("http://fpdownload.adobe.com/pub/swz/flex/4.6.0.23201/charts_4.6.0.23201.swz",
"http://fpdownload.adobe.com/pub/swz/crossdomain.xml",
"b07e6a4389510e372fc066449e5bcca0f689a3c5e51b5aae55e4a0eeae7f80a6",
"SHA-256",true,true,"default"),
new RSLData("charts_4.6.0.23201.swz",
"",
"b07e6a4389510e372fc066449e5bcca0f689a3c5e51b5aae55e4a0eeae7f80a6",
"SHA-256",true,true,"default")],
[new RSLData("http://fpdownload.adobe.com/pub/swz/flex/4.6.0.23201/spark_dmv_4.6.0.23201.swz",
"http://fpdownload.adobe.com/pub/swz/crossdomain.xml",
"d38dbc462676d679b79eb7d46887d34b5b33dfe9065751946ea104fd387ec69c",
"SHA-256",true,true,"default"),
new RSLData("spark_dmv_4.6.0.23201.swz",
"",
"d38dbc462676d679b79eb7d46887d34b5b33dfe9065751946ea104fd387ec69c",
"SHA-256",true,true,"default")],
[new RSLData("http://fpdownload.adobe.com/pub/swz/flex/4.6.0.23201/osmf_1.0.0.16316.swz",
"http://fpdownload.adobe.com/pub/swz/crossdomain.xml",
"b63185fca5d2bdbb568593f2bf232e87e5a20a7ea2ce2e26671d159838d598ed",
"SHA-256",true,true,"default"),
new RSLData("osmf_1.0.0.16316.swz",
"",
"b63185fca5d2bdbb568593f2bf232e87e5a20a7ea2ce2e26671d159838d598ed",
"SHA-256",true,true,"default")],
[new RSLData("http://fpdownload.adobe.com/pub/swz/flex/4.6.0.23201/advancedgrids_4.6.0.23201.swz",
"http://fpdownload.adobe.com/pub/swz/crossdomain.xml",
"529c81ac00092e51dcd0016be8ba25f6d54c55dd9a3a94f9a67d25a3d1ea6015",
"SHA-256",true,true,"default"),
new RSLData("advancedgrids_4.6.0.23201.swz",
"",
"529c81ac00092e51dcd0016be8ba25f6d54c55dd9a3a94f9a67d25a3d1ea6015",
"SHA-256",true,true,"default")]]
,
            preloader: mx.preloaders.SparkDownloadProgressBar
            }
        }
        return _info;
    }


    /**
     *  @private
     */
    private var _preloadedRSLs:Dictionary; // key: LoaderInfo, value: Vector.<RSLData>

    /**
     *  @private
     */
    private var _allowDomainParameters:Vector.<Array>;

    /**
     *  @private
     */
    private var _allowInsecureDomainParameters:Vector.<Array>;

    /**
     *  @private
     *  The RSLs loaded by this system manager before the application
     *  starts. RSLs loaded by the application are not included in this list.
     */
    override public function get preloadedRSLs():Dictionary
    {
        if (_preloadedRSLs == null)
           _preloadedRSLs = new Dictionary(true);
        return _preloadedRSLs;
    }

    /**
     *  @private
     *  Calls Security.allowDomain() for the SWF associated with this IFlexModuleFactory
     *  plus all the SWFs assocatiated with RSLs preLoaded by this IFlexModuleFactory.
     *
     */
    override public function allowDomain(... domains):void
    {
        Security.allowDomain.apply(null, domains);

        for (var loaderInfo:Object in _preloadedRSLs)
        {
            if (loaderInfo.content && ("allowDomainInRSL" in loaderInfo.content))
                loaderInfo.content["allowDomainInRSL"].apply(null, domains);
        }

        if (!_allowDomainParameters)
            _allowDomainParameters = new Vector.<Array>();
        _allowDomainParameters.push(domains);

        // Run our handler before the default handlers so the RSL is trusted before the
        // default handlers run.
        addEventListener(RSLEvent.RSL_ADD_PRELOADED, addPreloadedRSLHandler, false, 50);
    }

    /**
     *  @private
     *  Calls Security.allowInsecureDomain() for the SWF associated with this IFlexModuleFactory
     *  plus all the SWFs assocatiated with RSLs preLoaded by this IFlexModuleFactory.
     *
     */
    override public function allowInsecureDomain(... domains):void
    {
        Security.allowInsecureDomain.apply(null, domains);

        for (var loaderInfo:Object in _preloadedRSLs)
        {
            if (loaderInfo.content && ("allowInsecureDomainInRSL" in loaderInfo.content))
                loaderInfo.content["allowInsecureDomainInRSL"].apply(null, domains);
        }
        if (!_allowInsecureDomainParameters)
            _allowInsecureDomainParameters = new Vector.<Array>();
        _allowInsecureDomainParameters.push(domains);

        // Run our handler before the default handlers so the RSL is trusted before the
        // default handlers run.
        addEventListener(RSLEvent.RSL_ADD_PRELOADED, addPreloadedRSLHandler, false, 50);
    }

    /**
     *  @private
     */
    private function addPreloadedRSLHandler(event:RSLEvent):void
    {
        var loaderInfo:LoaderInfo = event.loaderInfo;
        if (!loaderInfo || !loaderInfo.content)
            return;
        var domains:Array
        if (allowDomainsInNewRSLs && _allowDomainParameters)
        {
            for each (domains in _allowDomainParameters)
            {
                if ("allowDomainInRSL" in loaderInfo.content)
                    loaderInfo.content["allowDomainInRSL"].apply(null, domains);
            }
        }

        if (allowInsecureDomainsInNewRSLs && _allowInsecureDomainParameters)
        {
            for each (domains in _allowInsecureDomainParameters)
            {
                if ("allowInsecureDomainInRSL" in loaderInfo.content)
                    loaderInfo.content["allowInsecureDomainInRSL"].apply(null, domains);
            }
        }
    }


}

}
