(function($) {
/**
 * @class Drupal.availabilityCalendar.BookingFormlet provides the glueing code
 *   that connects the reservation formlet with the availability calendar fields
 *   on the page.
 * @param Array[Integer] cids
 *   The list of calendar id's to listen to.
 * @param Integer bookedState
 *   The state id (sid) to visually change the state to after the user has
 *   clicked both an arrival and departure date. This gives the visitor visual
 *   feedback and suggest that the state changes to "optionally booked".
 * @param String instanceId
 *   The id of the formlet instance that this object instance is linked to.
 * @see AvailabilityCalendars API object.
 */
Drupal.availabilityCalendar.BookingFormlet = function(cids, bookedState, instanceId) {
  var cid = null;
  var calendarLabel = null;
  var from = null;
  var to = null;

  init();
  show();

  function init() {
    // Attach to the calendar click events from all calendars to serve.
    for (var i = 0; i < cids.length; i++) {
      Drupal.availabilityCalendar.get(cids[i]).getCalendar().bind('calendarclick', addDate);
    }

    // Attach to the click events of the reset buttons.
    $('.acbf-reset-from', instanceId).click(resetFrom);
    $('.acbf-reset-both', instanceId).click(resetBoth);
  }

  /**
   * Adds a date to the command.
   * - If it is the first date, it will be the from date.
   * - If it is the 2nd date, it will be the to date, swapping the from and to dates if needed.
   * - If it is a 3rd date, either the from or to date will be changed, depending on whether
   *   the 3rd date is before the current from or not.
   */
  function addDate(event, date, calendarCid) {
    if (cid !== calendarCid) {
      // Clean up old calendar (both the settings and visually).
      resetBoth();
      show();
      cid = calendarCid;
      $('input[name="cid"]', instanceId).val(cid);
      calendarLabel = Drupal.availabilityCalendar.get(cid).getName();
      $('input[name="calendar_label"]', instanceId).val(calendarLabel);
    }

    if (from === null) {
      from = date;
    }
    else {
      var shiftTo = true;
      if (to !== null) {
        Drupal.availabilityCalendar.get(cid).restoreRangeState(from, to);
        if (date < from) {
          from = date;
          shiftTo = false;
        }
        else {
          to = date;
        }
      }
      else {
        if (date >= from) {
          to = date;
        }
        else {
          to = from;
          from = date;
        }
      }

      if (Drupal.availabilityCalendar.get(cid).isSplitDay() && shiftTo) {
        // Split day: to date = departure date = am only: store as "from" to "to - 1 day".
        to.setDate(to.getDate() - 1);
        if (to < from) {
          // In the split day situation, clicking twice on the same day results
          // in an empty period: refuse.
          to = null;
        }
      }
    }
    show();
  };

  /**
   * Resets the from date and restores the calendar.
   */
  function resetFrom() {
    if (cid !== null) {
      if (from !== null && to === null) {
        Drupal.availabilityCalendar.get(cid).removeExtraState(from, 'cal-selected');
      }
      from = null;
      show();
    }
    return false;
  };

  /**
   * Resets both dates and restores the calendar.
   */
  function resetBoth() {
    if (cid !== null) {
      if (from !== null && to !== null) {
        Drupal.availabilityCalendar.get(cid).restoreRangeState(from, to);
      }
      from = to = null;
      show();
    }
    return false;
  }

  /**
   * @returns Boolean Whether the current form values are valid.
   */
  function isValid() {
    return cid !== null && to !== null && from !== null;
  };

  /**
   * Shows the current values, help texts, and enables the submit button.
   */
  function show() {
    if (from === null) {
      $('.form-reset', instanceId).css('display', 'none');
      $('.acbf-arrival', instanceId)
        .attr('disabled', 'disabled')
        .addClass('form-button-disabled')
        .val(Drupal.t('Click on an available date in the calendar'));
      $('.acbf-departure', instanceId)
        .val('')
        .attr('disabled', 'disabled')
        .addClass('form-button-disabled');
    }
    else {
      $('.acbf-arrival', instanceId)
        .val(from.toFormattedString('dd-mm-yyyy'))
        .removeAttr('disabled')
        .removeClass('form-button-disabled');
      if (to === null) {
        $('.acbf-reset-from', instanceId).css('display', 'inline-block');
        $('.acbf-reset-both', instanceId).css('display', 'none');
        $('.acbf-departure', instanceId)
          .val(Drupal.t('Click on your departure date'))
          .attr('disabled', 'disabled')
          .addClass('form-button-disabled');
        Drupal.availabilityCalendar.get(cid).addExtraState(from, 'cal-selected');
      }
      else {
        var showTo = new Date(to.getFullYear(), to.getMonth(), to.getDate() + 1);
        $('.acbf-departure', instanceId)
          .val(showTo.toFormattedString('dd-mm-yyyy'))
          .removeAttr('disabled')
          .removeClass('form-button-disabled');
        $('.acbf-reset-from', instanceId).css('display', 'none');
        $('.acbf-reset-both', instanceId).css('display', 'inline-block');
        // If to < from then calselected was set on to date, so clear both.
        Drupal.availabilityCalendar.get(cid).removeExtraState(from, 'cal-selected');
        Drupal.availabilityCalendar.get(cid).removeExtraState(Drupal.availabilityCalendar.get(cid).isSplitDay() ? showTo : to, 'cal-selected');
        if (isValid()) {
          Drupal.availabilityCalendar.get(cid).changeRangeState(from, to, bookedState);
        }
      }
    }
    if (isValid()) {
      $('.form-submit', instanceId).removeAttr('disabled').removeClass('form-button-disabled');

    }
    else {
      $('.form-submit', instanceId).attr('disabled', 'disabled').addClass('form-button-disabled');
    }

  };

  // Nothing to return (no public interface)
  return true;
};
})(jQuery);
