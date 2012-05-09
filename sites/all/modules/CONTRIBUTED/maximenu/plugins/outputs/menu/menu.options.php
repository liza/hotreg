<?php
/* $Id$ */

/**
 * @file
 * Maximenu Output - Menu
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Loads options to Maximenu admin
 *
 */  
function _maximenu_menu_options($menu_content = array(), $settings = array()) {

	$form_new = array();
  
	// Skin
  $output_options  = isset($settings['outputs']) ? $settings['outputs']: '';
	if (!empty($output_options)) {
    $form_new['stacking'] = array(
      '#type'  => 'select',
      '#title' => t('Stacking'),
      '#options' => array('row' => 'Row', 'column' => 'Column'),    
      '#default_value' => isset($menu_content['stacking']) ? $menu_content['stacking']: 'row',        
      '#description' => t('
        <ul>
          <li><strong>Row</strong> - all links in 1 row</li>
          <li><strong>Column</strong> -all links in 1 column</li>
        </ul>
       '), 
    ); 
    $form_new['direction'] = array(
      '#type'  => 'select',
      '#title' => t('Block Menu Direction'),
      '#options' => array('block-down' => 'Dropdown','block-up' => 'Dropup', 'block-right' => 'Right', 'block-left' => 'Left'),    
      '#default_value' => isset($menu_content['direction']) ? $menu_content['direction']: 'block-down', 
      '#description' => t('OM Maximenu will popup on this direction. Applicable only with dropdown style.'),                   
    );   
    $access = user_access('use PHP for settings');

    if (isset($menu_content['visibility']) && $menu_content['visibility'] == 2 && !$access) {
       $form_new['visibility'] = array('#type' => 'value', '#value' => 2);
       $form_new['pages'] = array('#type' => 'value', '#value' => $menu_content['pages']);
    }
    else {
      $options = array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.'));
      $description = t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcardfor every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>'));

      if (module_exists('php') && $access) {
        $options[] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
        $description .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code canbreak your Drupal site.', array('%php' => '<?php ?>'));
      }
      $form_new['visibility'] = array(
        '#type' => 'radios',
        '#title' => t('Show menu on specific pages'),
        '#options' => $options,
        '#default_value' => isset($menu_content['visibility']) ? $menu_content['visibility']: 0,
      );
      $form_new['pages'] = array(
        '#type' => 'textarea',
        '#title' => t('Pages'),
        '#default_value' => isset($menu_content['pages']) ? $menu_content['pages']: '',
        '#description' => $description,
      );
    }
	}
  return $form_new;				
}