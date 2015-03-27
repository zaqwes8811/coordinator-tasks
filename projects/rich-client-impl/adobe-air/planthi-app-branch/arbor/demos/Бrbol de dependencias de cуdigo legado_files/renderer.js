(function(){
  //Inicializa para dibujar
  Renderer = function(canvas){
    var canvas = $(canvas).get(0)
    var ctx = canvas.getContext("2d");
    var gfx = arbor.Graphics(canvas)
    var particleSystem = null
	var clic2veces='A';
	var dependenciasbd='';
	
    var that = {
      init:function(system){
        particleSystem = system
        particleSystem.screenSize(canvas.width, canvas.height) 
        particleSystem.screenPadding(40)

        that.initMouseHandling()
      },

      redraw:function(){
        if (!particleSystem) return

        gfx.clear() // inicia el canvas

        // dibuja los nodos y los nodos a los que apuntan
        var nodeBoxes = {}
        particleSystem.eachNode(function(node, pt){
          // node: {mass:#, p:{x,y}, name:"", data:{}}
          // pt:   {x:#, y:#}  cordenadas de la posicion del nodo

          
          var label = node.data.label||""
          var w = ctx.measureText(""+label).width + 10
          if (!(""+label).match(/^[ \t]*$/)){
            pt.x = Math.floor(pt.x)
            pt.y = Math.floor(pt.y)
          }else{
            label = null
          }

         
          if (node.data.tipo){
          /*ctx.fillStyle = node.data.color*/
          /***********/
            //validamos el tipo de nodo para ponerle color
 			if (node.data.tipo=="arbol"){
 			ctx.fillStyle="rgb(230,0,0)"
 			}
 			else if (node.data.tipo=="clase"){
 			ctx.fillStyle="rgb(255, 166, 0)"
 			}
 			else if (node.data.tipo=="constructore"){
 			ctx.fillStyle="rgb(0, 196, 255)"
 			}
 			else if(node.data.tipo=="metodo"){
 			ctx.fillStyle="rgb(158, 225, 81)"
 			}
 			else if(node.data.tipo=="variable"){
 			ctx.fillStyle="rgb(255,255,81)"
 			}
 			else
 			{
 			ctx.fillStyle="gray";
 			}
 			
 			
          }
          else ctx.fillStyle = "rgba(0,0,0,.2)"//en caso de que el tipo no se haya declarado
          if (node.data.color=='none') ctx.fillStyle = "white"

          if (node.data.shape=='dot'){//comenzaremos a dibujarlos en forma de circulo
            gfx.oval(pt.x-w/2, pt.y-w/2, w,w, {fill:ctx.fillStyle})
            nodeBoxes[node.name] = [pt.x-w/2, pt.y-w/2, w,w]
            // tamaño del borde
			ctx.shadowOffsetX = 2;
			ctx.shadowOffsetY = 2;
			// tamaño de la sombra
			ctx.shadowBlur = 6;
			// color de la sombra
			ctx.shadowColor = "gray";
			
            //Color de la linea de alrededor
			ctx.stroke();
      		ctx.strokeStyle="white";
      		
      		
			
          }else if (node.data.shape=='dependencia'){//caso de que sean dependencias
          gfx.oval(pt.x-w/2, pt.y-w/2, w,w, {fill:ctx.fillStyle})
            nodeBoxes[node.name] = [pt.x-w/2, pt.y-w/2, w,w]
            // tañano de la circuferencia
            ctx.shadowOffsetX = 2;
			ctx.shadowOffsetY = 2;
			// Define the blur width
			ctx.shadowBlur = 6;
			// Define the colour of the shadow
			ctx.shadowColor = "gray";
			
            
			ctx.stroke();
      		ctx.strokeStyle="white";
          }else{
            gfx.rect(pt.x-w/2, pt.y-10, w,20, 4, {fill:ctx.fillStyle})
            nodeBoxes[node.name] = [pt.x-w/2, pt.y-11, w, 22]
          }

          // draw the text
          if (label){
            ctx.font = "12px Helvetica"
            ctx.textAlign = "center"
            ctx.fillStyle = "white"
            if (node.data.color=='none') ctx.fillStyle = '#333333'
            ctx.fillText(label||"", pt.x, pt.y+4)
            ctx.fillText(label||"", pt.x, pt.y+4)
          }
        })    			


        // draw the edges
        particleSystem.eachEdge(function(edge, pt1, pt2){
          // edge: {source:Node, target:Node, length:#, data:{}}
          // pt1:  {x:#, y:#}  source position in screen coords
          // pt2:  {x:#, y:#}  target position in screen coords

          var weight = edge.data.weight
          var color = edge.data.color

          if (!color || (""+color).match(/^[ \t]*$/)) color = null

          // find the start point
          var tail = intersect_line_box(pt1, pt2, nodeBoxes[edge.source.name])
          var head = intersect_line_box(tail, pt2, nodeBoxes[edge.target.name])

          ctx.save() 
            ctx.beginPath()
            ctx.lineWidth = (!isNaN(weight)) ? parseFloat(weight) : 1
            ctx.strokeStyle = (color) ? color : "rgb(126, 135, 47)"
            
            ctx.fillStyle = null

            ctx.moveTo(tail.x, tail.y)
            ctx.lineTo(head.x, head.y)
            ctx.stroke()
          ctx.restore()

          // draw an arrowhead if this is a -> style edge
          //if (node.data.shape=='dependencia'){
          if (edge.data.directed){
            ctx.save()
              // move to the head position of the edge we just drew
              var wt = !isNaN(weight) ? parseFloat(weight) : 1
              var arrowLength = 6 + wt
              var arrowWidth = 2 + wt
              ctx.fillStyle = (color) ? color : "black"
              ctx.translate(head.x, head.y);
              ctx.rotate(Math.atan2(head.y - tail.y, head.x - tail.x));
			  ctx.strokeStyle = (color) ? color : "black"
			  ctx.strokeStyle = "black"
              // delete some of the edge that's already there (so the point isn't hidden)
              ctx.clearRect(-arrowLength/2,-wt/2, arrowLength/2,wt)

              // draw the chevron
              ctx.beginPath();
              ctx.moveTo(-arrowLength, arrowWidth);
              ctx.lineTo(0, 0);
              ctx.lineTo(-arrowLength, -arrowWidth);
              ctx.lineTo(-arrowLength * 0.8, -0);
              ctx.closePath();
              ctx.fill();
            ctx.restore()
          }
        })



      },
      initMouseHandling:function(){
        // agrega el manejador para los metodos del mouse
        selected = null;
        nearest = null;
        var dragged = null;
        var oldmass = 1

        //  inicializa un manejador del objeto para que escuche los eventos del clic
        var handler = {
          clicked:function(e){
          //funcion clic
            var pos = $(canvas).offset();
            _mouseP = arbor.Point(e.pageX-pos.left, e.pageY-pos.top)
            selected = nearest = dragged = particleSystem.nearest(_mouseP);

            if (dragged.node !== null) dragged.node.fixed = true

            $(canvas).bind('mousemove', handler.dragged)//le indica que puede arrastrarlo
            $(window).bind('mouseup', handler.dropped)//le indica que puede soltarlo, evento draganddrop
           $(canvas).bind('mouseup', handler.dobleclicked)//caso de que se haga dobleclic
            
            $(canvas).unbind('dblclick', handler.tripleclicked)//desactiva el dobleclic
            return false
          },
          dragged:function(e){
          //funcion arrastrar del mouse
            var old_nearest = nearest && nearest.node._id
            var pos = $(canvas).offset();
            var s = arbor.Point(e.pageX-pos.left, e.pageY-pos.top)

            if (!nearest) return
            if (dragged !== null && dragged.node !== null){
              var p = particleSystem.fromScreen(s)
              dragged.node.p = p
            }
			$(canvas).unbind('mouseup', handler.dobleclicked)
            
            return false
          },
          dropped:function(e){
          //funcion soltar del mouse
            if (dragged===null || dragged.node===undefined) return
            if (dragged.node !== null) dragged.node.fixed = false
            dragged.node.tempMass = 50
            dragged = null
            selected = null
            $(canvas).unbind('mousemove', handler.dragged)
            $(window).unbind('mouseup', handler.dropped)
            $(canvas).unbind('mouseup', handler.dobleclicked)
            $(canvas).unbind('dblclick', handler.tripleclicked) 
            _mouseP = null
            clic2veces='A';
            return false
          },
          dobleclicked:function(e){
          //caso de que se haga dobleclic, maneja el evento clic para mostrar los nodos hijos
         	if (dragged===null || dragged.node===undefined) return
          	if (dragged.node !== null){
          	
           		dragged.node.fixed = false
           		
           		var id=dragged.node.name;           		
           		
 				 $.ajax({
    			data: "pid="+id,
    			type: "GET",
    			dataType: "json",
    			url: "arbol.php",
    			success: function(data){
       			restults(data);
     			}
   				});  				
  				
				var restults = function(data){
     			/*$("div.descripcion").html("").show();
     			
       			$("div.info").append("Arbol: "+data.arbol);
       			$("div.info").append("Nodo: "+data.nodo);
       			$("div.info").append("Padre: "+data.padre);
       			$("div.info").append("Nombre: "+data.nombre);
       			$("div.info").append("Tipo: "+data.tipo);
       			$("div.descripcion").append("Impresión: "+data.lista);*/
       			
       			document.getElementById('descripcionnodos').innerHTML=''+ data.descdep;
       			
       			eval(data.nodes);
       			
  				};//var restults
          		
          		
          		
          		
          		}//nodo diferente de null
          		
          		//dragged = null
            	selected = null
            	$(canvas).unbind('mousemove', handler.dragged)
            	$(window).unbind('mouseup', handler.dropped)
            	$(canvas).unbind('mouseup', handler.dobleclicked)
            	$(canvas).bind('dblclick', handler.tripleclicked) 
            	_mouseP = null
            	
          		return false
          },//dobleclicked
           tripleclicked:function(e){
           //funcion para dobleclic que dibuja las flechas de dependencias a los nodos
           var id=dragged.node.name;
         
          		if (clic2veces==id)
          		{
          		
          		clic2veces='A';
          		}else{
          		
          		 $.ajax({
    			data: "pid="+id,
    			type: "GET",
    			dataType: "json",
    			url: "dependencias.php",
    			success: function(data){
       			restults(data);
     			}
   				});  				
  				
				var restults = function(data){
       			eval(data.dependencias);
  				};
          		clic2veces=id;
          		}
           
           
           
           
           dragged = null
            	selected = null
            	$(canvas).unbind('mousemove', handler.dragged)
            	$(window).unbind('mouseup', handler.dropped)
            	 
            	$(canvas).unbind('dblclick', handler.tripleclicked) 
            	_mouseP = null
            	
          		return false
           }
          
        }
        $(canvas).mousedown(handler.clicked);
       

      }      

    }

    // funcion que crea las lineas
    var intersect_line_line = function(p1, p2, p3, p4)
    {
      var denom = ((p4.y - p3.y)*(p2.x - p1.x) - (p4.x - p3.x)*(p2.y - p1.y));
      if (denom === 0) return false // en caso de lineas paralelas, no ocurre pero por si las dudas
      var ua = ((p4.x - p3.x)*(p1.y - p3.y) - (p4.y - p3.y)*(p1.x - p3.x)) / denom;
      var ub = ((p2.x - p1.x)*(p1.y - p3.y) - (p2.y - p1.y)*(p1.x - p3.x)) / denom;

      if (ua < 0 || ua > 1 || ub < 0 || ub > 1)  return false
      return arbor.Point(p1.x + ua * (p2.x - p1.x), p1.y + ua * (p2.y - p1.y));
    }

    var intersect_line_box = function(p1, p2, boxTuple)
    {
      var p3 = {x:boxTuple[0], y:boxTuple[1]},
          w = boxTuple[2],
          h = boxTuple[3]

      var tl = {x: p3.x, y: p3.y};
      var tr = {x: p3.x + w, y: p3.y};
      var bl = {x: p3.x, y: p3.y + h};
      var br = {x: p3.x + w, y: p3.y + h};

      return intersect_line_line(p1, p2, tl, tr) ||
            intersect_line_line(p1, p2, tr, br) ||
            intersect_line_line(p1, p2, br, bl) ||
            intersect_line_line(p1, p2, bl, tl) ||
            false
    }

    return that
  }    
  
})()
//funcion que oculta los nodos pertenencientes al nodo
function contraernodo(id){
document.getElementById('descripcionocultar').innerHTML='espere...';
	$.ajax({
    			data: "pid="+id,
    			type: "GET",
    			dataType: "json",
    			url: "nodosocultar.php",
    			success: function(data){
       			results(data);
     			}
   				});  				
  				
				var results = function(data){
			document.getElementById('descripcionocultar').innerHTML='contraer';
       			eval(data.nodosocultar);
       			
  				};         			
}
var abiertoocerrado=0;
function cerrardetalles(){
if (abiertoocerrado==0){
document.getElementById('mega_drop').style.display='block';	
$('info-detalles').addClass('active');
	abiertoocerrado=1;
}else
{
document.getElementById('mega_drop').style.display='none';
$('info-detalles').removeClass('active');	
abiertoocerrado=0;
}

	
}
function mostraracerca(){
	var popFullscreen = document.getElementById('fullscreen');
    var popacerca = document.getElementById('acercainfo');
	obtentamanho();
    
    popFullscreen.style.height = miScrollAlto + 'px';
    // Posiciona pop-up
    popacerca.style.left = ((miAncho - popacerca.offsetWidth) / 2) + 'px';
    popacerca.style.top = (((miAlto - popacerca.offsetHeight) / 2) + miScroll) + 'px';
    popFullscreen.style.display = 'block';
    popFullscreen.style.visibility='visible';
    popacerca.style.visibility = 'visible';
    popacerca.style.display='block';	
}
function cerraracerca(){
	var popFullscreen = document.getElementById('fullscreen');
    var popacerca = document.getElementById('acercainfo');
	obtentamanho();
   
    popFullscreen.style.height = miScrollAlto + 'px';
    // Posiciona pop-up
    popacerca.style.left = ((miAncho - popacerca.offsetWidth) / 2) + 'px';
    popacerca.style.top = (((miAlto - popacerca.offsetHeight) / 2) + miScroll) + 'px';
     popFullscreen.style.display = 'none';
    popFullscreen.style.visibility='hidden';
    popacerca.style.visibility = 'hidden';
    popacerca.style.display='none';
    
}
function obtentamanho() {

	// Tamanho de ventana

	if (self.innerHeight) { // todos excepto IE
		miAncho = window.innerWidth;
		miAlto = window.innerHeight;
		miScroll = window.pageYOffset;
	} else if (document.documentElement && document.documentElement.clientHeight) { // solo IE6
		miAncho = document.documentElement.clientWidth;
		miAlto = document.documentElement.clientHeight;
		miScroll = document.documentElement.scrollTop;
	} else if (document.body) { // Otro IE, como IE7
		miAncho = document.body.clientWidth;
		miAlto = document.body.clientHeight;
		miScroll = document.body.scrollTop;
	}

	// tamanho de pagina w/areas no visibles

	if (window.innerHeight && window.scrollMaxY) {	
		miScrollAncho = document.body.scrollWidth;
		miScrollAlto = window.innerHeight + window.scrollMaxY;
	} else if (document.body.scrollHeight > document.body.offsetHeight) { // Todos excepcion navegadores Mac
		miScrollAncho = document.body.scrollWidth;
		miScrollAlto = document.body.scrollHeight;
	} else { // Exploradores Mac...deberia tambien trabajar en Explorer 6, Mozilla y Safari
		miScrollAncho = document.body.offsetWidth;
		miScrollAlto = document.body.offsetHeight;
	}
}
function mostrararbol(){
	$.ajax({
    			data: "pid=0",
    			type: "GET",
    			dataType: "json",
    			url: "nodosmostrar.php",
    			success: function(data){
       			results(data);
     			}
   				});  				
  				
				var results = function(data){
			
       			eval(data.nodosmostrar);
       			
  				};  
	
}