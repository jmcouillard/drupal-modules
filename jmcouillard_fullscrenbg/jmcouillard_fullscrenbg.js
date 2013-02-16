(function($, Drupal, window, document, undefined) {

	var fullscreenBackground;
	var fullscreenBackgroundCurrent;
	var fullscreenBackgroundTimerDelay;
	var fullscreenBackgroundTimer;

	// Init
	$.fn.fullscreenBackground = function(data, animtype, delay) {

		fullscreenBackground = data;
		fullscreenBackgroundTimerDelay = delay;

		options = {
			width : 1,
			height : 1,
			bgID : '#fullscreen',
			topOffset : 0,
			bottomOffset : 0,
			showAll : false
		};

		// Init Fullscreen
		$.fn.fullscreenr(options);

		// Start anim
		if (animtype == "fade") {
			$.fn.fullscreenBackgroundLoad(0);
			fullscreenBackgroundTimer = setInterval(function() {
				$.fn.fullscreenBackgroundNext()
			}, delay)
		} else if (animtype == "static") {
			$.fn.fullscreenBackgroundLoad(Math.floor(Math.random() * fullscreenBackground.length));
		}

		// Resize
		// $(window).bind("fullscreenBackgroundLoad", $.fn.tetraiteur_front_resize);

	};

	// Load image
	$.fn.fullscreenBackgroundLoad = function(id) {

		fullscreenBackgroundCurrent = id;

		$.preload(fullscreenBackground[id].url, {
			onFinish : function() {

				// $.fn.tetraiteur_front_resize();

				$(options.bgID).fadeOut(300, function() {

					options.width = fullscreenBackground[id].width;
					options.height = fullscreenBackground[id].height;
					$("#fullscreen").attr("src", fullscreenBackground[id].url);
					$.fn.fullscreenr(options);
					$(options.bgID).fadeIn(400);
					// $(window).trigger("resize");

				});
			}

		});
	}

	// Load next image
	$.fn.fullscreenBackgroundNext = function() {
		var id = (fullscreenBackgroundCurrent + 1) % fullscreenBackground.length;
		$.fn.fullscreenBackgroundLoad(id);
	}

})(jQuery, Drupal, this, this.document);
