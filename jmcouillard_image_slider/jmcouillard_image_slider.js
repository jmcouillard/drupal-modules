(function($){
	
	var lastIndex = -1;
	var currentIndex = -1;
	var delay = 8000;
	var timer;
	var imgCount = 0;
	
		
	$.fn.jmcouillard_image_slider = function (_delay, _width, _height) {
		
		delay = _delay;
		imgCount = $("#jmcouillard_image_slider img").length;
		
		// Replace all z
		$("#jmcouillard_image_slider img").each(function(i,n){
			$(this).css("zIndex", 10);
			if(i>0)$(this).hide();
		});

		// Reszie
		if(_width != "auto" && _height != "auto") $("#jmcouillard_image_slider").width(_width).height(_height);

		// Animate
		if(imgCount>1)  {
			$("#jmcouillard_image_slider").append("<div class='arrow next'></div>");
			$("#jmcouillard_image_slider .next").click(next);
			$("#jmcouillard_image_slider").append("<div class='arrow prev'></div>");
			$("#jmcouillard_image_slider .prev").click(prev);

			$("#jmcouillard_image_slider img").click(next);
		}

		loadImage(0);
	}
	
	var next = function () {
		loadImage((currentIndex+1)%imgCount);			
	}
	
	var prev = function () {
		var id = (currentIndex-1);
		if(id<0) id = imgCount-1;

		loadImage(id);			
	}
	
	var loadImage = function (id) {

		// Set variables
		lastIndex = currentIndex;
		currentIndex = id;

		
		// Show new one
		$("#jmcouillard_image_slider img").eq(currentIndex).css("zIndex", 1).css("position", "relative").show();

		//Fade out current
		if(lastIndex != -1) $("#jmcouillard_image_slider img").eq(lastIndex).css("position", "absolute").css("zIndex", 10).fadeOut(300, function(){
			if(lastIndex != -1) $("#jmcouillard_image_slider img").eq(lastIndex).hide();
			$(this).css("position", "relative");
		});

		// Refresh label
		refreshLabel();
		
		// Restart timer
		clearTimeout(timer);
		if(imgCount>1) timer = setTimeout(next, delay);				
	}

	var refreshLabel = function(){

		var line1 = $("#jmcouillard_image_slider img").eq(currentIndex).attr("alt");
		var line2 = $("#jmcouillard_image_slider img").eq(currentIndex).attr("title");

		$("#jmcouillard_image_slider .label").html("<div class='line1'>" + line1 + "</div><div class='line2'>" + line2 + "</div>");
	};
	
})(jQuery);
