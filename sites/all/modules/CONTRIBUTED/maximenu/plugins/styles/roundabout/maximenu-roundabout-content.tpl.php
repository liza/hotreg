<?php
/**
 * @file maximenu-roundabout-content.tpl.php
 * Default theme implementation of om maximenu contents with roundabout blocks
 *
 * Available variables:
 * - $content: array, used for link classes and content
 *
 * Helper variables:
 * - $zebra: Same output as $block_zebra but independent of any block region.
 * - $id: Same output as $block_id but independent of any block region.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 * - $user: (object) user properties
 *
 * @see template_preprocess_maximenu_roundabout()
 * @see template_preprocess_maximenu_roundabout_links()
 * @see template_preprocess_maximenu_roundabout_content()
 *
<?php if (!empty($content)): ?>
  <div class="maximenu-roundabout-content">
    <?php print _maximenu_content_render($content); ?>
    <div class="clearfix"></div>  
    <div class="maximenu-arrow"></div>
  </div><!-- /.maximenu-roundabout-content -->
<?php endif; ?> 
 */
?> 

<?php if (!empty($content)): ?>
  <div class="maximenu-roundabout-content">
    <div class="maximenu-top">
      <div class="maximenu-top-left"></div>
      <div class="maximenu-top-right"></div>
    </div><!-- /.maximenu-top --> 
    <div class="maximenu-middle">
      <div class="maximenu-middle-left">
        <div class="maximenu-middle-right">
          <?php print _maximenu_content_render($content); ?>
          <div class="clearfix"></div>
        </div><!-- /.maximenu-middle-right --> 
      </div><!-- /.maximenu-middle-left --> 
    </div><!-- /.maximenu-middle --> 
    <div class="maximenu-bottom">
      <div class="maximenu-bottom-left"></div>
      <div class="maximenu-bottom-right"></div>
    </div><!-- /.maximenu-bottom -->  
    <div class="maximenu-arrow"></div>
  </div><!-- /.maximenu-content -->  
<?php endif; ?> 
