(function($) {
/**
 * @class Drupal.availabilityCalendar.Command represents a calendar state
 * changing command during the whole creation phase, i.e. from click on a state
 * to the click on the end date.
 */
Drupal.availabilityCalendar.Command = function(cid, fieldContext) {
  this.state = "";
  this.from = null;
  this.to = null;
  this.elt = $(".availability-changes", fieldContext);

  /**
   * Sets the state of the current command and, as this is supposed to be the
   * first parameter to be set, cleans the from and to dates.
   */
  this.setState = function(selectedState) {
    if (selectedState !== undefined) {
      this.state = selectedState;
    }
    this.from = null;
    this.to = null;
    this.show();
  };

  /**
   * Adds a date to the command. If it is the 1st date it will be the from date.
   * If it is the 2nd date it will be the to date, if necessary, swapping the
   * from and to dates.
   *
   * @param Date date
   */
  this.addDate = function(date) {
    if (this.from === null) {
      this.from = date;
    }
    else if (this.to === null) {
      this.to = date;
      if (this.to < this.from) {
        var _from = this.from;
        this.from = this.to;
        this.to = _from;
      }
      if (Drupal.availabilityCalendar.get(cid).isSplitDay()) {
        // Split day: to date = departure date = am only: store as "from" to "to - 1 day".
        this.to.setDate(this.to.getDate() - 1);
        if (this.to < this.from) {
          // In the split day situation, clicking twice on the same day results
          // in an empty period: refuse.
          this.to = null;
        }
      }
    }
    this.show();
  };

  /**
   * @returns Boolean Whether the current command is complete.
   */
  this.isComplete = function() {
    return this.to !== null && this.from !== null && this.state !== '';
  };

  /**
   * Replaces the current command in the accompanying hidden field.
   */
  this.show = function() {
    var val = this.elt.val();
    var pos = val.lastIndexOf("\n") + 1;
    val = val.substr(0, pos) + this.toString();
    this.elt.val(val);
  };

  /**
   * Finishes the current command and starts a new one.
   */
  this.finish = function() {
    this.show();
    this.elt.val(this.elt.val() + "\n");
    this.setState();
  };

  /**
   * @returns String
   *   A string representation of the current command.
   */
  this.toString = function() {
    result = "";
    result += "state: ";
    result += this.state !== "" ? this.state : "-";
    result += " from: ";
    result += this.from !== null ? this.from.toFormattedString("yyyy-mm-dd") : "-";
    result += " to: ";
    result += this.to !== null ? this.to.toFormattedString("yyyy-mm-dd") : "-";
    return result;
  };
};

/**
 * @class Drupal.availabilityCalendar.Edit provides the glueing code that
 * connects the form elements on entity edit forms (for entities with an
 * availability calendar field) with the @see AvailabilityCalendar API object
 * and the @see Drupal.availabilityCalendar.Command class.
 */
Drupal.availabilityCalendar.Edit = function(cid) {
  var calendar = Drupal.availabilityCalendar.get(cid);
  var fieldContext = calendar.getCalendar().parents('.form-wrapper').first();
  var formRadios = $(".form-radios.availability-states", fieldContext);
  var enable = $('.availability-enable', fieldContext);

  // Initialize command.
  var command = new Drupal.availabilityCalendar.Command(cid, fieldContext);
  command.setState($(":radio:checked", formRadios).val());
  // Add css_class of states as class to wrapper elements around the radios.
  $(":radio", formRadios).parent().addClass(function() {
    return Drupal.availabilityCalendar.states.getClass($(this).children(":radio:").val());
  });

  // Attach to enable checkbox (if it exists).
  if (enable.size() > 0 ) {
    $('.availability-details', fieldContext).toggle(enable.filter(':checked').size() > 0);
    enable.click(function () {
      $('.availability-details', fieldContext).toggle('fast');
    });
  }

  // Attach to state radios events.
  $("input:radio", formRadios).click(function() {
    // State clicked: remove cal-selected (if set) and restart current command.
    if (command.from !== null) {
      calendar.removeExtraState(command.from, "cal-selected");
    }
    command.setState($(":radio:checked", formRadios).val());
  });

  // Attach to the calendar events.
  calendar.getCalendar().bind("calendarclick", function(event, date, cid) {
    command.addDate(date);
    if (!command.isComplete()) {
      calendar.addExtraState(command.from, "cal-selected");
    }
    else {
      calendar.removeExtraState(command.from, "cal-selected");
      // Remove selected state from to date as well, but take differences into
      // account between split day and whole day handling.
      var to = new Date(command.to.getTime());
      if (calendar.isSplitDay()) {
        to.setDate(to.getDate() + 1);
      }
      calendar.removeExtraState(to, "cal-selected");
      calendar.changeRangeState(command.from, command.to, command.state);
      command.finish();
    }
  });

  return null;
};
})(jQuery);
