<?php
/**
 * @file maximenu-wrapper.tpl.php
 * Default theme implementation of om maximenu wrapper
 *
 * Available variables:
 * - $maximenu_name: menu title
 * - $links: array, link properties
 * - $link_code: menu unique identifier
 * - $content: rendered content
 *
 * @see template_preprocess_maximenu_wrapper()
 *
 */
?> 

<?php if (!empty($content)): ?>
  <div id="maximenu-<?php print $maximenu_name; ?>" class="maximenu<?php print _maximenu_classes($links); ?> code-<?php print $link_code; ?>"<?php print _maximenu_inline_style($links); ?>>     
    <?php if($links['output'] == 'float'): ?>
      <div id="maximenu-<?php print $maximenu_name; ?>-inner" class="maximenu-wrapper-inner">
        <?php print $content; ?>
      </div><!-- /.maximenu-wrapper-inner -->
    <?php else: ?>
      <?php print $content; ?>
    <?php endif; ?>
  </div><!-- /#maximenu-[menu name] -->   
<?php endif; ?>


