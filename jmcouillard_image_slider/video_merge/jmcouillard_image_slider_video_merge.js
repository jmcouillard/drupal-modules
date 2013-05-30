
(function($){

	var clearTimerInterval;
		
	$.fn.jmcouillard_image_slider_video_merge = function (_width, _height) {

		// Merge image and videos
		$($("#jmcouillard_image_slider_video_merge iframe").get().reverse()).each(function(i,n){
			$(this).css("width", _width);
			$(this).css("height", _height);
			$(this).prependTo("#jmcouillard_image_slider .wrapper");
		});

		// Listen for messages from the player VIMEO
		if (window.addEventListener) window.addEventListener('message', onMessageReceived, false);
		else window.attachEvent('onmessage', onMessageReceived, false);
	}

	var clearTimer = function() {
		$.fn.jmcouillard_image_slider_clear_timer();
	}

	// YOUTUBE functions
	window.onYouTubeIframeAPIReady = function () {
		var youtubes = $("#jmcouillard_image_slider iframe.youtube").each(function(i,n){
			 var player = 	new YT.Player($(this).attr("id"), {
		          events: {
		            'onStateChange': onPlayerStateChange
		          }
		    });		
		});
	};

	function onPlayerStateChange(event) {
        if(event.data == 1) {
        	clearTimerInterval = setInterval(clearTimer, 1000);
        } else {
        	clearInterval(clearTimerInterval);
        }
      }

	// VIMEO functions
	function onMessageReceived(e) {

	    var data = JSON.parse(e.data);
		var item = $("#jmcouillard_image_slider #" + data.player_id).first();

	    if(data.event == "ready") {
		    // post('addEventListener', 'pause', item);
		    // post('addEventListener', 'finish', item);
		    post('addEventListener', 'playProgress', item);
	    }

		if(data.event == "playProgress") {
		    clearTimer();
	    }
    }

    function post(action, value, item) {
		var url = item.attr('src').split('?')[0];	    
	    var data = { method: action };
	    if (value) data.value = value;	    
	    item[0].contentWindow.postMessage(JSON.stringify(data), url);
	}
	
})(jQuery);
