/**
 * README for Availability Calendar module.
 *
 * @author Erwin Derksen (fietserwin: http://drupal.org/user/750928)
 * @link: http://drupal.org/project/availability_calendars (project page)
 *
 * Note that the module name is availability_calendar, thus without an s, though
 * the project page and the tar.gz do have an s. This is for historical reasons.
 */

The availability calendar module defines an availability calendar field. It is
a complete rewrite of version 7.x-2.x of the Availability Calendars module.
This version is based on the fields feature of D7.


Dependencies
------------
- The Views support part uses the format_string function that is available as of
  Drupal 7.9.
- The Views support part uses the date_popup if available, but this is no hard
  dependency.


Installing
----------
As usual. After enabling the module:
- Define the states you want to use on
  admin/config/content/availability-calendar/settings
- Define the basic styling, including the colors for the states, on
  admin/config/content/availability-calendar/styling
- Add availability calendar fields to the requested content types.


Styling
-------
The modules contains a style sheet with basic styling that should give you a
reasonable look & feel out of the box (availability_calendar.base.css).
Additionally, you can specify some styling via the admin user interface on
admin/config/content/availability-calendar/styling. This will generate a file
sites/default/files/availability_calendar/availability_calendar.css. Remaining
styling is to be defined in your theme. See availability_calendar.base.css for
how the calendar and key are rendered.


Upgrading from Availability Calendars 7.x-2.x or earlier
--------------------------------------------------------
To Drupal this is a different module from the already existing Availability
Calendars module. This makes upgrading via update.php a bit tricky. Therefore, a
separate update module has been created. This module can be found in the latest
7.x-2.x package. So install that version as well. The Availability Calendars
update module contains an UPGRADE.txt with more detailed information about
upgrading.


Views integration
-----------------
Views integration has been added. Note that although there is a separate filter
on enabled in the views UI, if you define a view that accesses information from
one of the availability_calendar_* tables, an extra join condition on enabled
will be added automatically. So normally there is no need to add this filter,
except perhaps in some administrative edge use cases.


Search on availability
----------------------
Through the views integration it is now possible to search on availability. Just
add a filter "<field name> available". The dates that are filled in, probably
exposed to the visitor, are the arrival and departure dates. Thus the arrival
date is inclusive but the departure date is non-inclusive. The UI can use some
improvement, but that's for the next version.


Caching
-------
Caching pages with availability calendars is possible but keep in mind that the
calendars change just because the date changes, thus without anyone changing the
data that belongs to the calendar. This means that ideally you should set your
page caches to expire next midnight. However, most caching mechanisms, including
the standard one provided by Drupal, only allows you to set an offset to the
current time. So an offset up to half a day should not give you many problems.
Note that in a multilingual set-up with field syncing (i18n_sync module) field
syncing goes through node_save and thus invalidates the cache.


I18n
----
Availability calendar is (or strives to be) fully multilingual aware. Using the
standard translation model - several entities composing 1 translation set - the
calendars can be shared between translations by enabling field syncing for them.

The names of the states are considered hard coded texts and thus translated
using t() not i18n_string, even though they may be overridden via user entered
input. They should thus be entered in English.

The names of the calendars are field values and thus not translated. On syncing
they won't overwrite already existing names, but if no name exists in the target
language the name is copied.


API
---
A number of functions of this module can be used by other modules. Together
these functions provide the base methods to use availability calendars without
providing any UI. The primary target for external use is probably the function
availability_calendar_query_available() which allows others to provide a query
which will be extended with conditions to filter on availability. Useful if you
have your own form to search on availability.

To make use of the API you have to include the .inc file:
  module_load_include('inc', 'availability_calendar', 'availability_calendar');
