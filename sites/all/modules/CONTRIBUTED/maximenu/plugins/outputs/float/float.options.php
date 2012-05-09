<?php
/* $Id$ */

/**
 * @file
 * Maximenu Output - Float
 *
 * @author: Daniel Honrade http://drupal.org/user/351112
 *
 */

/**
 * Loads options to Maximenu admin
 *
 */  
function _maximenu_float_options($menu_content = array(), $settings = array()) {

	$form_new = array();
  
	// Skin
  $output_options  = isset($settings['outputs']) ? $settings['outputs']: '';
	if (!empty($output_options)) {
    $form_new['position'] = array(
      '#type'  => 'select',
      '#title' => t('Fixed / Absolute'),
      '#options' => array('fixed' => 'Fixed', 'absolute' => 'Absolute'),    
      '#default_value' => isset($menu_content['position']) ? $menu_content['position']: 'fixed',
      '#description' => t('
        <ul>
          <li><strong>Fixed</strong> - stays on screen on scrolling</li>
          <li><strong>Absolute</strong> - moves with the page on scrolling</li>
        </ul>
       '),         
    );     
    $form_new['y_origin'] = array(
      '#type'  => 'select',
      '#title' => t('Y-Origin: Top / Bottom'),
      '#options' => array('top' => 'Top', 'bottom' => 'Bottom'),    
      '#default_value' => isset($menu_content['y_origin']) ? $menu_content['y_origin']: 'bottom',        
    ); 
    $form_new['y_value'] = array(
      '#type'  => 'textfield',
      '#title' => t('Y-Axis Value'),
      '#size'  => 5,
      '#default_value' => isset($menu_content['y_value']) ? $menu_content['y_value']: 10,        
    );
    $form_new['x_origin'] = array(
      '#type'  => 'select',
      '#title' => t('X-Origin: Left / Middle / Right'),
      '#options' => array('left' => 'Left', 'middle' => 'Middle', 'right' => 'Right'),    
      '#attributes' => array('class' => array('om-maximenu-x-origin')),
      '#default_value' => isset($menu_content['x_origin']) ? $menu_content['x_origin']: 'left',        
    ); 
    $form_new['non_middle_options'] = array(
      '#type'  => 'fieldset',
      '#title' => t('Non-Middle Options'),
      '#collapsible' => TRUE,
      '#collapsed'   => TRUE, 
      '#attributes' => array(
        'class' => array('om-maximenu-non-middle-options'), 
        'style' => (isset($menu_content['x_origin']) && ($menu_content['x_origin'] == 'middle')) ? 'display: none;': 'display: block;',
      ),    
    );  
    $form_new['non_middle_options']['x_value'] = array(
      '#type'  => 'textfield',
      '#title' => t('X-Axis Value'),
      '#size'  => 5,    
      '#default_value' => isset($menu_content['x_value']) ? $menu_content['x_value']: 10,        
      '#attributes' => array('class' => array('om-maximenu-x-value')),
    );
    $form_new['non_middle_options']['stacking'] = array(
      '#type'  => 'select',
      '#title' => t('Stacking'),
      '#options' => array('row' => 'Row', 'column' => 'Column'),    
      '#default_value' => isset($menu_content['stacking']) ? $menu_content['stacking']: 'row',        
      '#attributes' => array('class' => array('om-maximenu-stacking')),
      '#description' => t('
        <ul>
          <li><strong>Row</strong> - all links in 1 row</li>
          <li><strong>Column</strong> -all links in 1 column</li>
        </ul>
       '), 
    ); 
    $form_new['non_middle_options']['orientation'] = array(
      '#type'  => 'select',
      '#title' => t('Orientation'),
      '#options' => array('horizontal' => 'Horizontal', 'vertical' => 'Vertical'),    
      '#default_value' => isset($menu_content['orientation']) ? $menu_content['orientation']: 'horizontal',        
      '#attributes' => array('class' => array('om-maximenu-orientation')),
      '#description' => t('
        <ul>
          <li><strong>Horizontal</strong> - normal text/icon orientation</li>
          <li><strong>Vertical</strong> - left(90deg rotation), right(270deg rotation)</li>
        </ul>
      '), 
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