<?php
/**
 * @file maximenu-tab-links.tpl.php
 * Default theme implementation of om maximenu links with tabbed blocks
 *
 * Available variables:
 * - $maximenu_name: Menu name given on configuration 
 * - $link: rendered span or a tag with attributes
 * - $content: array, used for link classes and content
 *
 * Helper variables:
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $user: (object) user properties
 * - $count: link counter
 * - $total: number of links
 * - $permission: TRUE/FALSE
 * - $key: numeric link order id
 * - $code: unique id given in the system
 *
 * @see template_preprocess_maximenu_tab()
 * @see template_preprocess_maximenu_tab_links()
 * @see template_preprocess_maximenu_tab_content()
 *
 */
?>  

<?php if (!empty($permission)): ?>   
  <li id="leaf-<?php print $code . '-' . $key; ?>" class="<?php print _maximenu_link_classes($content, $permission, $count, $total); ?>">   
    <?php print $link; ?>
  </li>
<?php endif; ?>  
