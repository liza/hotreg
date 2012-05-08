(function($) {
/**
 * Helper method that extends the String object with a padLeft method
 */
String.prototype.padLeft = function(value, size) {
  var x = this;
  while (x.length < size) {
    x = value + x;
  }
  return x;
};

/**
 * Helper method that extends the Array object with a top method
 */
Array.prototype.top = function() {
  return this[this.length - 1];
};

/**
 * Helper method that extends the Date object with a toFormattedString method to allow
 * for easier printing of dates. (Reduced version of one that was found on
 * Stackoverflow.)
 */
Date.prototype.toFormattedString = function(format) {
  return format
    .replace(/yyyy/g, this.getFullYear())
    .replace(/mm/g, String(this.getMonth() + 1).padLeft("0", 2))
    .replace(/m/g, String(this.getMonth() + 1))
    .replace(/dd/g, String(this.getDate()).padLeft("0", 2))
    .replace(/d/g, this.getDate());
};

Drupal.availabilityCalendar = {};

/**
 * @class States
 *   Represents all availability states. Per state some information is kept:
 *   - Integer sid,
 *   - String cssClass
 *   - Boolean isAvailable
 * @param Array states
 *   Array with information about all possible states.
 * @returns object
 *   States object.
 */
Drupal.availabilityCalendar.States = function(states) {
  var _statesById = {};
  var _statesByClass = {};

  _init();

  function _init() {
    // Add empty state (easiest way to prevent javascript errors).
    states[""] = {sid: 0, css_class: "", is_available: false};
    for (var key in states) {
      if (states.hasOwnProperty(key)) {
        _statesById[states[key].sid] = states[key];
        _statesByClass[states[key].css_class] = states[key];
      }
    }
  }

  function getAll() {
    return _statesById;
  }

  function get(state) {
    if (isNaN(state) && _statesByClass[state] !== undefined) {
      return _statesByClass[state];
    }
    else if (!isNaN(state) && _statesById[state] !== undefined) {
      return _statesById[state];
    }
    return {};
  }

  function getClass(state) {
    return get(state)['css_class'] !== undefined ? get(state)['css_class'] : '';
  }

  function isAvailable(state) {
    return get(state)['is_available'] !== undefined ? get(state)['is_available'] : false;
  }

  return {
    getAll: getAll,
    get: get,
    getClass: getClass,
    isAvailable: isAvailable
  };
};

/**
 * Javascript API for Availability Calendar field module.
 *
 * @class AvailabilityCalendar
 *   Represents the client-side interface for a given availability calendar.
 *   It is possible to have multiple calendars on the same page.
 *   The cid parameter is used to distinghuish calendars.
 *
 *   This API class features:
 *   - Some methods to change the (visual) status of calendar days. However,
 *     note that it does not update the server-side calendar.
 *   - It defines a 'calendarclick' event and triggers it when the visitor
 *     clicks on a day cell in the calendar. The 'calendarclick' event passes in
 *     a Date object, representing the day that was clicked on, and the cid, to
 *     identify which calendar was clicked on. As only DOM elements can trigger
 *     events, you must bind your custom event handler to the calendar element,
 *     retrieved via AvailabilityCalendar.getCalendar():
 *     @code
 *     myAvailabilityCalendar.getCalendar().bind('calendarclick', function(event, date, cid) {
 *       alert('You clicked on date ' + date + ' of calendar ' + cid);
 *     });
 *     @endcode
 *
 * @constructor
 *   Creates a new AvailabilityCalendar interaction object.
 * @param int|string cid
 *   The calendar id for the calendar we want to interact with.
 *   A string starting with 'new' for not yet existing calendars.
 * @param boolean splitDay
 *   Indicates whether this calendar should be visualized using split days.
 * @param string selectable
 *   none|all|available|not-available
 *   Indicates whether this calendar allows interaction by selecting dates,
 *   and if so, what states may be selected
 */
Drupal.availabilityCalendar.Constructor = function(cid, name, splitDay, selectable) {
  var AM = 1;
  var PM = 2;
  var WHOLE_DAY = 3;
  var _calendarId = "#cal-" + cid;
  var _calendarRange = null;
  var _calendarDays = {};
  _init();

  /**
   * Initializes the calendar.
   * - Gives selectable cells the class selectable
   * - Initializes the custom calendarclick event on these cells
   */
  function _init() {
    _initDaysAdministration();
    _initSelectable();
    _initCustomEvents();
  };

  /**
   * Creates an overview of all days, their DOM element and their states
   */
  function _initDaysAdministration() {
    // Get all calendar months
    $(".cal-month", getCalendar())
      .filter(function() {
        // Make sure it is a calendar month
        return $(this).attr("id").split("-", 4).length == 4;
      })
      .each(function() {
        // Get year and month of this calendar month
        var day = null;
        var idParts = $(this).attr("id").split("-", 4);
        var year = idParts[2];
        var month = idParts[3];
        // Get all day cells of this calendar month
        $("tbody td", $(this))
          .not(".cal-other, .cal-pastdate")
          .each(function() {
            var cell = $(this);
            // Performance of the expresssion Number(cell.text()) is not best,
            // so use the fact that the set is ordered:
            // http://docs.jquery.com/Release:jQuery_1.3.2#Elements_Returned_in_Document_Order
            day = day === null ? Number(cell.text()) : day + 1;
            var date = (new Date(year, month - 1, day)).toFormattedString("yyyy-mm-dd");
            var currentClass = this.className;
            var state = {am: "", pm: ""};
            // Loop through all availability states (not including inherited properties)
            // to extract the availability state from the class.
            // Remove found classes to arrive at the other 'extra' classes.
            for (var key in getStateSettings().getAll()) {
              var cssClass = getStateSettings().getClass(key);
              if (cssClass !== '') {
                if (cell.hasClass(cssClass)) {
                  // Distribute over am and pm but do not overwrite.
                  state.am = state.am || cssClass;
                  state.pm = state.pm || cssClass;
                  cell.removeClass(cssClass);
                }
                if (cell.hasClass(cssClass + "-am")) {
                  state.am = cssClass;
                  cell.removeClass(cssClass + "-am");
                }
                if (cell.hasClass(cssClass + "-pm")) {
                  state.pm = cssClass;
                  cell.removeClass(cssClass + "-pm");
                }
              }
            }
            state = state.am === state.pm ? state.am : state;
            _calendarDays[date] = {cell: this, states: [state], extraStates: this.className};
            // Restore original class
            this.className = currentClass;
          });
      });
  };

  /**
   * Makes certain cells selectable.
   */
  function _initSelectable() {
    if (selectable !== "none") {
      for (var day in _calendarDays) {
        switch (selectable) {
          case "all":
            addExtraState(day, "cal-selectable");
            break;
          case "available":
            if (isAvailable(day, AM) || isAvailable(day, PM)) {
              addExtraState(day, "cal-selectable");
            }
            break;
          case "not-available":
            if (!isAvailable(day, AM) || !isAvailable(day, PM)) {
              addExtraState(day, "cal-selectable");
            }
            break;
        }
      }
    }
  };

  /**
   * Initializes the custom events for this calendar.
   *
   * The events are triggered by the calendar element, the div surrounding this calendar.
   * Other javascript thus should bind to that element (retrieved via @see getCalendar()).
   *
   * Currently provided custom events:
   * - calendarclick: comes with a date object and the cid as parameters.
   */
  function _initCustomEvents() {
    getCalendar().click(function(event) {
      // Find out if event originated from a day cell, get the date,
      // and trigger the event on the calendar element.
      var day, month, year;
      var cell = $(event.target).closest("td.cal-selectable");
      if (cell.size() > 0) {
        cell
          .each(function() {
            day = Number($(this).text());
          })
          .closest(".cal-month")
          .filter(function() {
            return $(this).attr("id").split("-", 4).length == 4;
          })
          .each(function() {
            var idParts = $(this).attr("id").split("-", 4);
            year = idParts[2];
            month = idParts[3];
          })
          .closest(_calendarId)
          .triggerHandler("calendarclick", [new Date(year, month - 1, day), cid]);
      }
    });
  };

  /**
   * @returns jQuery
   *   A jQuery object containing the calendar DOM element, that is the div with
   *   id='cal-{cid}' and class='cal' surrounding this calendar. This element
   *   triggers the custom events, thus other javascript should bind its
   *   calendar event handling to the return value of this method.
   */
  function getCalendar() {
    return $(_calendarId);
  };

  /**
   * @returns Integer
   *   The cid of the calendar.
   */
  function getCid() {
    return cid;
  };

  /**
   * @returns String
   *   The name of the calendar.
   */
  function getName() {
    return name;
  };

  /**
   * @returns Boolean
   *   If the calendar is shown using split days.
   */
  function isSplitDay() {
    return splitDay;
  };

  /**
   * @returns Drupal.availabilityCalendar.States
   */
  function getStateSettings() {
    return Drupal.availabilityCalendar.states;
  };

  /**
   * Returns the date range of the calendar.
   *
   * @returns Object
   *   { from: Date, to: Date }
   */
  function getCalendarRange() {
    if (_calendarRange === null) {
      var from, to;
      _calendarRange = {from: new Date(0, 0, 1), to: new Date(0, 0, 1)};
      // Get all tables representing calendar months within this calendar,
      // extract the month and update the from/to range.
      $(".cal-month", getCalendar()).each(function() {
        var idParts = $(this).attr("id").split("-", 4);
        if (idParts.length == 4) {
          var year = idParts[2];
          var month = idParts[3];
          var calFrom = new Date(year, month - 1, 1);
          if (from === undefined || from > calFrom) {
            from = calFrom;
          }
          var calTo = new Date(year, month, 0);
          if (to === undefined || to < calTo) {
            to = calTo;
          }
        }
      });
      _calendarRange = {from: from, to: to};
    }
    return _calendarRange;
  };

  /**
   * Returns whether the given date is within the calendar range.
   *
   * @param Date day
   * @returns Boolean
   */
  function isInCalendarRange(day) {
    var range = getCalendarRange();
    return range.from <= day && day <= range.to;
  };

  /**
   * Returns the number of months the calendar displays.
   *
   * @returns Integer
   */
  function getNumberOfMonths() {
    var range = getCalendarRange();
    return (range.to.getFullYear() - range.from.getFullYear()) * 12
      + range.to.getMonth() - range.from.getMonth() + 1;
  };

  /**
   * Returns the state for a given date.
   *
   * @param Date day
   * @returns null|Integer|Object
   *   - Integer for a whole day state.
   *   - object for a split day state: { am: int, pm: int }.
   *   - null for a date not within the calendar range.
   */
  function getState(day) {
    var state = null;
    var date = typeof day === "string" ?  day : day.toFormattedString("yyyy-mm-dd");
    if (_calendarDays[date] !== undefined) {
      state = _calendarDays[date].states.top();
    }
    return state;
  };

  /**
   * Indicates whether a day state is to be considered available for the given dayPart.
   *
   * @param Integer|String|Object state
   *   A single whole day state (Integer|String) or
   *   A composed am/pm split state (Object): { am: Integer|String, pm: Integer|String }.
   * @param Integer dayPart optional (defaults to whole day)
   *   1 = am
   *   2 = pm
   *   3 = am + pm = whole day
   * @returns Boolean
   *   true if the state is available.
   *   null when the state is not recognised.
   */
  function _isDayStateAvailable(state, dayPart) {
    var available;
    // Ignore dayPart if calendar does not support split days.
    if (dayPart === undefined || !splitDay) {
      dayPart = WHOLE_DAY;
    }
    if (typeof state === "string") {
      available = getStateSettings().isAvailable(state);
    }
    else {
      available = true;
      if ((dayPart & AM) !== 0) {
        // am state is to be taken into account.
        available = getStateSettings().isAvailable(state.am);
      }
      if ((dayPart & PM) !== 0) {
        // pm state is to be taken into account.
        available = available && getStateSettings().isAvailable(state.pm);
      }
    }
    return Boolean(available);
  };

  /**
   * Returns whether the given day is available for the given dayPart.
   *
   * @parma Date date
   * @param Integer dayPart optional (defaults to whole day)
   *   1 = am
   *   2 = pm
   *   3 = am + pm = whole day
   * @returns Boolean
   *   true if the (part of the) date is available.
   *   false if the (part of the) date is not available.
   *   null if the date is not within the calendar range.
   */
  function isAvailable(date, dayPart) {
    var available = null;
    var state = getState(date);
    if (state !== null) {
      available = _isDayStateAvailable(state, dayPart);
    }
    return available;
  };

  /**
   * Internal function to combine the state and any extra states
   * for the given day to 1 value for the className property.
   *
   * @param Date date
   */
  function _setCellClass(date) {
    var state = _calendarDays[date].states.top();
    var cssClass = typeof state === "string" ? state : state.am + "-am " + state.pm + "-pm";
    if (_calendarDays[date].extraStates !== '') {
      cssClass += " " + _calendarDays[date].extraStates;
    }
    _calendarDays[date].cell.className = cssClass;
  };

  /**
   * Changes the availability state of the given (part of the) day.
   *
   * @param Date day
   * @param Integer|String state
   * @param Integer dayPart optional (defaults to whole day)
   *   1 = am
   *   2 = pm
   *   3 = am + pm = whole day
   */
  function changeState(day, state, dayPart) {
    var date = typeof day === "string" ?  day : day.toFormattedString("yyyy-mm-dd");
    state = getStateSettings().getClass(state);
    if (_calendarDays[date] !== undefined) {
      // Ignore dayPart if calendar does not support split days.
      if (dayPart === undefined || !splitDay) {
        dayPart = WHOLE_DAY;
      }

      // Determine new composite state: get a clone of the current state ...
      var newState = _calendarDays[date].states.top();
      if (typeof newState === "string") {
        newState = {am: newState, pm: newState};
      }
      else {
        newState = {am: newState.am, pm: newState.pm};
      }
      // ... and overwrite the dayPart's to change
      if ((dayPart & AM) !== 0) {
        newState.am = state;
      }
      if ((dayPart & PM) !== 0) {
        newState.pm = state;
      }
      newState = newState.am === newState.pm ? newState.am : newState;
      _calendarDays[date].states.push(newState);
      // And set the class on the accompanying cell
      _setCellClass(date);
    }
  };

  /**
   * Restores the availability state of the given (part of the) day to its previous value.
   *
   * @param Date day
   */
  function restoreState(day) {
    var date = typeof day === "string" ?  day : day.toFormattedString("yyyy-mm-dd");
    if (_calendarDays[date] !== undefined) {
      // Remove current state (if not the original state)
      if (_calendarDays[date].states.length > 1) {
        _calendarDays[date].states.pop();
      }
      // And set the class on the accompanying cell
      _setCellClass(date);
    }
  };

  /**
   * Sets an extra state on the given (part of the) day.
   * Extra states do not mix with or replace the availability settings.
   *
   * @param Date|String day
   *   Date object or yyyy-mm-dd string
   * @param String extraState
   */
  function addExtraState(day, extraState) {
    var date = typeof day === "string" ?  day : day.toFormattedString("yyyy-mm-dd");
    if (_calendarDays[date] !== undefined) {
      // Only add if extra state is not already set.
      var extraStates = " " + _calendarDays[date].extraStates + " ";
      if (extraStates.indexOf(" " + extraState + " ") < 0) {
        _calendarDays[date].extraStates = jQuery.trim(extraStates + " " + extraState);
        // And set the class on the accompanying cell.
        _setCellClass(date);
      }
    }
  };

  /**
   * Removes an extra state of the given (part of the) day.
   * Extra states do not mix with or replace the availability settings.
   *
   * @param Date day
   * @param String state
   */
  function removeExtraState(day, extraState) {
    var date = typeof day === "string" ?  day : day.toFormattedString("yyyy-mm-dd");
    if (_calendarDays[date] !== undefined) {
      // Remove state (if set).
      var extraStates = " " + _calendarDays[date].extraStates + " ";
      extraStates = extraStates.replace(" " + extraState + " ", " ");
      _calendarDays[date].extraStates = jQuery.trim(extraStates);
      // And set the class on the accompanying cell.
      _setCellClass(date);
    }
  };

  /**
   * Returns whether all dates in the given range are available.
   * In the split day situation we check from 'from pm' to 'to am'.
   * In the whole day situation we check from 'from' up to but not including 'to'.
   *
   * @param Date from
   * @param Date to
   * @returns Boolean|null
   *   true if the whole range is available,
   *   false if not the whole range is available,
   *   null if the given date range is not fully within the calendar range.
   */
  function isRangeAvailable(from, to) {
    var available = null;
    if (!splitDay) {
      // We don't have to check for the last day itself.
      to.setDate(to.getDate() - 1);
    }
    if (isInCalendarRange(from) && isInCalendarRange(to)) {
      available = true;
      var day = new Date(from.getTime());
      var dayPart = PM;
      while (available && day <= to) {
        available = isAvailable(day, dayPart);
        day.setDate(day.getDate() + 1);
        dayPart = day >= to ? AM : WHOLE_DAY;
      }
    }
    return available;
  };

  /**
   * Sets all days in the from - to range to the given state.
   * In the split day situation we change from 'from pm' to 'to + 1 day am'.
   * In the whole day situation we change from 'from' up to and including 'to'.
   *
   * @param Date from
   * @param Date to
   * @param Integer|String state
   */
  function changeRangeState(from, to, state) {
    var calendarRange = getCalendarRange();
    if (calendarRange.from > from) {
      from = calendarRange.from;
    }
    if (calendarRange.to < to) {
      to = calendarRange.to;
    }
    var day = new Date(from.getTime());
    var dayPart = isSplitDay() ? PM : WHOLE_DAY;
    while (day <= to) {
      changeState(day, state, dayPart);
      day.setDate(day.getDate() + 1);
      dayPart = WHOLE_DAY;
    }
    if (isSplitDay() && calendarRange.to >= day) {
      changeState(day, state, AM);
    }
  };

  /**
   * Restores all states for the days in the from - to range to their previous state.
   * In the split day situation we change from 'from pm' up to 'to + 1 day am'.
   * In the whole day situation we change from 'from' up to and including 'to'.
   *
   * @param Date from
   * @param Date to
   */
  function restoreRangeState(from, to) {
    var calendarRange = getCalendarRange();
    if (calendarRange.from > from) {
      from = calendarRange.from;
    }
    if (calendarRange.to < to) {
      to = calendarRange.to;
    }
    var day = new Date(from.getTime());
    while (day <= to) {
      restoreState(day);
      day.setDate(day.getDate() + 1);
    }
    if (isSplitDay() && calendarRange.to >= day) {
      restoreState(day);
    }
  };

  /**
   * Adds the given extra state to all days in the from - to range.
   * In the split day situation we change from 'from pm' to 'to am'.
   * In the whole day situation we change from 'from' up to and including 'to'.
   * Extra states do not mix with or replace the availability settings.
   *
   * @param Date from
   * @param Date to
   * @param String state
   */
  function addRangeExtraState(from, to, state) {
    var calendarRange = getCalendarRange();
    if (calendarRange.from > from) {
      from = calendarRange.from;
    }
    if (calendarRange.to < to) {
      to = calendarRange.to;
    }
    var day = new Date(from.getTime());
    while (day <= to) {
      addExtraState(day, state);
      day = day.setDate(day.getDate() + 1);
    }
  };

  /**
   * Removes the given state from all days in the from - to range.
   * In the split day situation we remove from 'from pm' to 'to am'.
   * In the whole day situation we remove from 'from' up to and including 'to'.
   * Extra states do not mix with or replace the availability settings.
   *
   * @param Date from
   * @param Date to
   * @param String state
   */
  function removeRangeExtraState(from, to, state) {
    var calendarRange = getCalendarRange();
    if (calendarRange.from > from) {
      from = calendarRange.from;
    }
    if (calendarRange.to < to) {
      to = calendarRange.to;
    }
    var day = new Date(from.getTime());
    while (day <= to) {
      removeExtraState(day, state);
      day = day.setDate(day.getDate() + 1);
    }
  };

  return {
    // Publicly exposed methods:
    getCalendar: getCalendar,
    getCid: getCid,
    getName: getName,
    isSplitDay: isSplitDay,
    getCalendarRange: getCalendarRange,
    isInCalendarRange: isInCalendarRange,
    getNumberOfMonths: getNumberOfMonths,
    getState: getState,
    isAvailable: isAvailable,
    changeState: changeState,
    restoreState: restoreState,
    addExtraState: addExtraState,
    removeExtraState: removeExtraState,
    isRangeAvailable: isRangeAvailable,
    changeRangeState: changeRangeState,
    restoreRangeState: restoreRangeState,
    addRangeExtraState: addRangeExtraState,
    removeRangeExtraState: removeRangeExtraState
  };
};

/**
 * @property object _calendars Collection of instances
 */
Drupal.availabilityCalendar._calendars = {};

/**
 * Multiton implementation for accessing calendars on the page
 */
Drupal.availabilityCalendar.get = function(cid, name, splitday, selectable) {
  if (Drupal.availabilityCalendar._calendars[cid] === undefined) {
    Drupal.availabilityCalendar._calendars[cid] = new Drupal.availabilityCalendar.Constructor(cid, name, splitday, selectable);
  }
  return Drupal.availabilityCalendar._calendars[cid];
};

/**
 * @returns A list of all calendars on the current page indexed by cid.
 */
Drupal.availabilityCalendar.getAll = function() {
  return Drupal.availabilityCalendar._calendars;
};

})(jQuery);
