package {
    import flash.net.URLLoader;
    import flash.net.URLRequest;
    import flash.display.*;
    import flash.events.*;
    import flash.text.*;
 
    public class Main extends Sprite {
        private var api_id:Number=743131; // id вашего приложения
        private var api_secret:String="sE4VlnFfSQ"; // секретный ключ
        private var externalXML:XML;
        private var loader:URLLoader = new URLLoader();
        private var photoLoader:Loader = new Loader();
        private var viewer_id:Number;
        private var sig:String;
        private var uid:String;
        private var bdate_txt:TextField = new TextField();
        private var name_txt:TextField = new TextField();
 
        public function Main():void {
            // узнаём id пользователя, который просматривает приложение
            var params:Object=LoaderInfo(root.loaderInfo).parameters;
            viewer_id = (params['viewer_id']) ? parseInt(params['viewer_id']) : 1;
            // получаем профиль этого пользователя
            getProfile(viewer_id);
        }
 
        private function getProfile(uid):void { // функция получения профиля
            sig = MD5.encrypt(
			  viewer_id + 'api_id=' +
			  api_id + 'fields=bdate,photo_bigmethod=getProfilesuids=' +
			  uid + 'v=2.0' + api_secret);
			  
			//
            var request:URLRequest = new URLRequest(
			  "http://api.vkontakte.ru/api.php?api_id=" +
			  api_id + "&method=getProfiles&uids=" +
			  uid+"&fields=bdate,photo_big&v=2.0&sig="+sig);
            loader.load(request);
            loader.addEventListener(Event.COMPLETE, onComplete);
        }
 
		/// /// ///
        private function onComplete(event:Event):void { // функция выполняется когда профиль был загружен
            bdate_txt.text='';
            if (loader!=null) {
                externalXML=new XML(loader.data);
                // отображаем дату рождения
                var bdate = externalXML..bdate; 
                bdate_txt.text=bdate;
                bdate_txt.x=490;
                bdate_txt.y=380;
                addChild(bdate_txt);
               
                // отображаем имя и фамилию
                var uname = externalXML..first_name + ' ' + externalXML..last_name;
                name_txt.text=uname;
                name_txt.x=410;
                name_txt.y=360;
                addChild(name_txt);
               
                // загружаем аватар
                var photo = externalXML..photo_big;
                var request2:URLRequest=new URLRequest(photo);
                photoLoader.load(request2);
                photoLoader.contentLoaderInfo.addEventListener(Event.COMPLETE, onLoaded);
 
            } else {
                trace("Error!");
            }
        }
 
        private function onLoaded(e:Event):void {
            // отображаем загруженный аватар
            photoLoader.x = (550-photoLoader.width)/2;
            photoLoader.y = (400-photoLoader.height)/2;
            addChild(photoLoader);
        }
    }
}