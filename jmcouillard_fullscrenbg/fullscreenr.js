/**
 * Fullscreenr - lightweight full screen background jquery plugin
 * By Jan Schneiders
 * Version 1.0
 * www.nanotux.com
 **/
(function($) {

	var options;

	$.fn.fullscreenr = function(_options) {
		if (_options.height === undefined)
			alert('Please supply the background image height, default values will now be used. These may be very inaccurate.');
		if (_options.width === undefined)
			alert('Please supply the background image width, default values will now be used. These may be very inaccurate.');
		if (_options.bgID === undefined)
			alert('Please supply the background image ID, default #bgimg will now be used.');

		var defaults = {
			width : 1280,
			height : 1024,
			bgID : 'bgimg',
			topOffset : 0,
			bottomOffset : 0,
			leftOffset : 0,
			rightOffset : 0,
			minbrowserwidth : 0,
			showAll : false
		};

		options = $.extend({}, defaults, _options);
		$(window).bind("load", function() {
			$(options.bgID).fullscreenrResizer();
		});
		$(window).bind("resize", function() {
			$(options.bgID).fullscreenrResizer();
		});
		return this;
	};

	$.fn.fullscreenrResizer = function() {

		// Set bg size
		var ratio = options.height / options.width;

		// Get browser window size
		var browserwidth = $(window).width() - options.rightOffset - options.leftOffset;
		var browserheight = $(window).height() - options.topOffset - options.bottomOffset;

		if (browserwidth < options.minbrowserwidth)
			browserwidth = options.minbrowserwidth;

		// Scale the image
		if ((browserheight / browserwidth) > ratio) {

			// Portrait
			if (!options.showAll) {
				$(this).height(browserheight);
				$(this).width(browserheight / ratio);
			} else {
				$(this).width(browserwidth);
				$(this).height(browserwidth * ratio);
			}

		} else {

			// Paysage
			if (!options.showAll) {
				$(this).width(browserwidth);
				$(this).height(browserwidth * ratio);
			} else {
				$(this).height(browserheight);
				$(this).width(browserheight / ratio);
			}
		}

		// Center the image
		$(this).css('left', (browserwidth - $(this).width()) / 2 + options.leftOffset);
		//$(this).css('top', (browserheight - $(this).height())/2 + options.topOffset);
		$(this).css('bottom', -options.bottomOffset);
		return this;
	};

	$.fn.fullscreenrUnbind = function() {
		$(window).unbind("resize");
	};

	$.fn.fullscreenrUpdate = function(_options) {
		options = $.extend({}, options, _options);
		$(options.bgID).fullscreenrResizer();
	};

})(jQuery); 