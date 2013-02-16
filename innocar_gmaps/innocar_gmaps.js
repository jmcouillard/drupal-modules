(function($){
			
	$.fn.innocar_gmaps = function () {

		var innoPos = new google.maps.LatLng(45.933568, -73.346675);

		 var styles = [{
		    "stylers": [{ "saturation": -40 }]
		 }];


    	var map = new google.maps.Map(document.getElementById("innocar_gmaps"), {  
				zoom: 12,
				maxZoom: 18,
				minZoom: 3,
				center: innoPos,
         		mapTypeId: google.maps.MapTypeId.ROADMAP,
         		disableDefaultUI: true,
         		zoomControl: true,
         		zoomControlOptions: {
         			position:google.maps.ControlPosition.LEFT_CENTER,
         			style: google.maps.ZoomControlStyle.LARGE,
         		}
		});

		map.setOptions({styles: styles})

   		var marker = new google.maps.Marker({
        	position: innoPos,
        	map: map,
  			title:"Innocar"
    	});
    		
    };	

})(jQuery);
