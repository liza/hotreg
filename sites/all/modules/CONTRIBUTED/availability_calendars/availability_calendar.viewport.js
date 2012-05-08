(function($) {
/**
 * @class Drupal.availabilityCalendar.Viewport
 *
 * @constructor Creates a new Drupal.availabilityCalendar.Viewport object.
 * @param Integer cid
 *   Required: the cid of the calendar to operate on.
 * @param object settings
 *   Optional: object with the following properties
 *   (name type (default) description):
 *   {
 *     month
 *       width     int (0) the width of 1 calendar month, used to calculate the width of the
 *                         viewport. If 0, use CSS to define the width of the viewport.
 *       height    int (0) the height of 1 calendar month, used to calculate the height of the
 *                         viewport. If 0, use CSS to define the height of the viewport.
 *     cols        int (3) The number of months that is shown horizontally in the viewport.
 *     rows        int (2) The number of months that is shown vertically in the viewport.
 *     scroll      int (0) indicating how many rows or cols to scroll.
 *     animate     Animation parameters, @see http://api.jquery.com/animate/
 *       backward  Advanced usage, do not pass in.
 *       forward   Advanced usage, do not pass in.
 *       speed     int|string ('slow') The animation speed, see jquery documentation of animate().
 *     totalMonths int (calculated) Advanced usage, do not pass in.
 *     firstMonth  int (calculated) Advanced usage, do not pass in.
 *   }
 */
Drupal.availabilityCalendar.Viewport = function(cid, settings) {
  var calendar = Drupal.availabilityCalendar.get(cid);
  var buttonBack = null;
  var buttonForward = null;
  var viewport = null;
  _init();

  /**
   * Initializes the viewport.
   */
  function _init() {
    _initSettings();
    _initDimensions();
    _initHandlers();
  };

  /**
   * Extends the settings with their defaults and adds the animations.
   */
  function _initSettings() {
    settings = $.extend(true, {
      month: {width: 0, height: 0},
      cols: 3,
      rows: 2,
      scroll: 1,
      animate: {speed: "slow"},
      totalMonths: calendar.getNumberOfMonths(),
      firstMonth: 1
    }, settings);

    if (settings.month.width == 0) {
      // Get outer width of a month wrapper.
      // If the calendar is initially hidden, e.g. in a collapsed fieldset or a
      // tab, the width and height cannot be easily retrieved. Therefore, a nice
      // small jQuery plugin - actual - is used to get the actual outerWidth and
      // outerHeight regardless current visibility. To save an additional
      // request, this plugin is added to the end of this file.
      settings.month.width = $(".cal-month", calendar.getCalendar()).actual('outerWidth', {includeMargin: true});
    }
    if (settings.month.height == 0) {
      settings.month.height = $(".cal-month", calendar.getCalendar()).actual('outerHeight', {includeMargin: true});
    }

    // @todo: subtract right/bottom margin from 1 month to get an even better
    // viewport width and height.
    if (!settings.animate.backward) {
      settings.animate.backward = settings.rows > 1
        ? { top: "+=" + (settings.scroll * settings.month.height) }
        : { left: "+=" + (settings.scroll * settings.month.width) };
    };
    if (!settings.animate.forward) {
      settings.animate.forward = settings.rows > 1
        ? { top: "-=" + (settings.scroll * settings.month.height) }
        : { left: "-=" + (settings.scroll * settings.month.width) };
    };
  }

  /**
   * Sets the dimensions of the viewport.
   */
  function _initDimensions() {
    viewport = $(".cal-viewport", calendar.getCalendar());
    if (settings.month.width != 0) {
      viewport.width(settings.cols * settings.month.width);
    }
    if (settings.month.height != 0) {
      viewport.height(settings.rows * settings.month.height);
    }
    // Move to the inner viewport as that is the element to be animated.
    viewport = viewport.children();
    // If scrolling horizontally, the inner viewport should be infinitely wide.
    if (settings.rows == 1) {
      viewport.width(10000);
    }
  }

  /**
   * Initialize event handlers for our own buttons.
   */
  function _initHandlers() {
    buttonBack = $(".cal-backward", calendar.getCalendar());
    buttonForward = $(".cal-forward", calendar.getCalendar());
    buttonBack.click(function() { scrollBackward(); });
    buttonForward.click(function() { scrollForward(); });
    setEnabledState();
  }

  /**
   * Set the enabled/disabled state of the clickable elements.
   *
   * This function uses the disabled attribute. Although officially meant for
   * form elements (probably buttons in our cases), it can be used on <a> tags
   * as well. In combination with the CSS properties cursor and pointer-events
   * (CSS3) you can visibly/effectively disable an <a> tag:
   * a[disabled] { cursor: auto; pointer-events: none }
   */
  function setEnabledState() {
    if (settings.firstMonth <= 1) {
      buttonBack.attr("disabled", "disabled");
    }
    else {
      buttonBack.removeAttr("disabled");
    }
    if (settings.firstMonth + settings.rows * settings.cols > settings.totalMonths) {
      buttonForward.attr("disabled", "disabled");
    }
    else {
      buttonForward.removeAttr("disabled");
    }
  }

  /**
   * Scroll the viewport backward (if possible).
   */
  function scrollBackward() {
    if (settings.firstMonth > 1) {
      viewport.animate(settings.animate.backward, settings.animate.speed);
      settings.firstMonth -= settings.rows > 1 ? settings.scroll * settings.cols : settings.scroll;
      setEnabledState();
    }
  };

  /**
   * Scroll the viewport forward (if possible).
   */
  function scrollForward() {
    if (settings.firstMonth + settings.rows * settings.cols <= settings.totalMonths) {
      viewport.animate(settings.animate.forward, settings.animate.speed);
      settings.firstMonth += settings.rows > 1 ? settings.scroll * settings.cols : settings.scroll;
      setEnabledState();
    }
  };

  return {
    // Publicly exposed methods for those who define their own buttons.
    scrollBackward: scrollBackward,
    scrollForward: scrollForward
  };
};
})(jQuery);

/* The following copyright and license message only holds for the code below. */
/* Copyright 2011, Ben Lin (http://dreamerslab.com/)
* Licensed under the MIT License (LICENSE.txt).
*
* Version: 1.0.4
*
* Requires: jQuery 1.2.3+
*/
(function(a){a.fn.extend({actual:function(b,k){var c,d,h,g,f,j,e,i;if(!this[b]){throw'$.actual => The jQuery method "'+b+'" you called does not exist';}h=a.extend({absolute:false,clone:false,includeMargin:undefined},k);d=this;if(h.clone===true){e=function(){d=d.filter(":first").clone().css({position:"absolute",top:-1000}).appendTo("body");};i=function(){d.remove();};}else{e=function(){c=d.parents().andSelf().filter(":hidden");g=h.absolute===true?{position:"absolute",visibility:"hidden",display:"block"}:{visibility:"hidden",display:"block"};f=[];c.each(function(){var m={},l;for(l in g){m[l]=this.style[l];this.style[l]=g[l];}f.push(m);});};i=function(){c.each(function(m){var n=f[m],l;for(l in g){this.style[l]=n[l];}});};}e();j=d[b](h.includeMargin);i();return j;}});})(jQuery);
