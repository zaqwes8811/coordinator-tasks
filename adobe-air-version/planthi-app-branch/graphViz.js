function plotGraph( recordsList ) {
	var sys = arbor.ParticleSystem(100, 100,1);
	sys.parameters({gravity:true});
	
	// на что рисуем
	sys.renderer = Renderer("#viewport") ;
	
	// добавляем
	var ani = sys.addNode(recordsList[0].id,{'color':'gold','shape':'rect','label':recordsList[0].Date});
	var dog = sys.addNode(recordsList[7].id,{'color':'blue','shape':'rect','label':recordsList[1].Date});
	var cat = sys.addNode(recordsList[2].id,{'color':'blue','shape':'rect','label':recordsList[2].Date});

	sys.addEdge(ani, dog);
	sys.addEdge(ani, cat);
}