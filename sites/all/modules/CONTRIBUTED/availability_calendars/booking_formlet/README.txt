/**
 * README for Availability Calendar Booking Formlet module.
 *
 * @author Erwin Derksen (fietserwin: http://drupal.org/user/750928)
 */

The availability calendar booking formlet module defines a booking formlet
field that cooperates with Availability Calendar fields. It offers a small form
that can receive an arrival and departure date by reacting to click events on
Availability Calendar fields. If both dates are filled in, the submit button is
enabled and the user can continue to the real booking form.

This form is called a formlet as it is a small minimal form that only receives
the arrival and departure dates. The full booking form, where the visitor can
enter its name and other details is to defined separately. Think off an "Add to
cart" form where one can enter the quantity and click on "add to cart" which
brings you to the full cart form.

Dependencies
------------
There are no hard module dependecies, but without the Availability Calendar
field this module is completely useless. Also the form will normally be posted
to a webform. So installing the webform module is recommended. 

Installing
----------
As usual.

Configuring
-----------
 After enabling the module:
- Add a webform to post the form to.
- Define the components that the visitor should complete like name, e-mail,
  address, numbner of persons, etc.
- Define components for the dates that are already filled in:
  - Arrival date (default value: %post[arrival]): the arrival date.
  - Departure date (default value: %post[departure]): the departure date.
  This will typicaly be disabled textfields, the visitor can not change the
  dates anymore as the availability calendar is not visible on the webform.
- As you will typically post to the same webform for all accommodations, you
  need to know from wich accommodation the request was coming. To this end, you
  may want to define additional components that receive this information needed
  to process the boooking:
  - cid (default value: %post[cid]): the calendar id.
  - calendar_label (default value: %post[calendar_label]): title of the calendar.
  - entity type (default value: %post[entity_type]): the entity type.
  - entity id (default value: %post[entity_id]): the ID of the entity.
  - entity label (default value: %post[entity_label]): the title of the entity.
  With the exception of entity label and/or calendar_label, these will typically
  be hidden fields as the visitor does not have to see this information.
- Fields from the POST that you don't need, don't have to be defined as a field.
  Alternatively, if you do need them for internal processing but do not want to
  send them in an e-mail, you can exclude them from the e-mail in the "Edit
  e-mail settings" page of the webform module.

- Add the booking formlet field to the entities/content types that have an
  Availability Calendar field.
- Style the form as you like (no default styling yet).

- Depending on your situation, processing the webform may need some additional
  programming from your side. Think of extracting the name of the accommodation
  from the entity, or getting the e-mail address to send the reqeust to. The
  webform module and the mail system offer hooks for this.
 
Limitations
-----------
This module will not:
- Change the calendar. When clicking on the departure date the calendar will be
  updated visually, but this change will not be stored on the server. You can
  define a hook for that. See e.g. the webform documentation to find out which
  (submission) hooks best fit your needs.
- Define the webform. To dependent on your specific situation.
- Offer payment integration.
