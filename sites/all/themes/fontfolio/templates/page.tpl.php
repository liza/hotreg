<?php
/**
 * @file
 * Default theme implementation to display a single Drupal page.
 */
?>
<div id="main_container">
  <div id="header_top">
    <div class="header_top_border"></div>
    <?php if ($social_links): ?>
      <div class="social">
        <?php print $social_links; ?>
        <div class="clear"></div>
      </div><!--//social-->
    <?php endif; ?>
    
    <?php if ($page['search']): ?>
      <div class="search">
        <?php print render($page['search']); ?>
      </div>
    <?php endif; ?>
    
    <?php if ($main_menu): ?>
      <div class="menu_cont">
        <?php print theme('links__system_main_menu', array(
          'links' => $main_menu,
          'attributes' => array(
            'id' => 'menu',
            'class' => array('links', 'inline', 'clearfix', 'menu'),
          ),
          'heading' => array(
            'text' => t('Main menu'),
            'level' => 'h2',
            'class' => array('element-invisible'),
          )));
        ?>
      </div><!--//menu_cont-->
    <?php endif; ?>
    
    <div class="clear"></div>
  </div> <!-- /#header_top --> 
  
  <div id="header"><div class="section clearfix">

    <?php if ($logo): ?>
      <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home" class="logo">
        <img src="<?php print $logo; ?>" alt="<?php print t('Home'); ?>" />
      </a>
    <?php endif; ?>

    <?php if ($site_name || $site_slogan): ?>
      <div id="name-and-slogan">
        <?php if ($site_name): ?>
          <?php if ($title): ?>
            <div id="site-name"><strong>
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </strong></div>
          <?php else: /* Use h1 when the content title is empty */ ?>
            <h1 id="site-name">
              <a href="<?php print $front_page; ?>" title="<?php print t('Home'); ?>" rel="home"><span><?php print $site_name; ?></span></a>
            </h1>
          <?php endif; ?>
        <?php endif; ?>

        <?php if ($site_slogan): ?>
          <div id="site-slogan"><?php print $site_slogan; ?></div>
        <?php endif; ?>
      </div> <!-- /#name-and-slogan -->
    <?php endif; ?>

    <?php if ($secondary_menu): ?>
      <div id="header_menu"><div class="section">
        
        <?php print theme('links__system_secondary_menu', array(
                'links' => $secondary_menu,
                'attributes' => array(
                  'id' => 'secondary-menu',
                  'class' => array('menu'),
                ),
              'heading' => array(
                'text' => t('Secondary menu'),
                'level' => 'h2',
                'class' => array('element-invisible'),
              ))); ?>
      </div></div> <!-- /.section, /#header_menu -->
    <?php endif; ?>

    </div></div> <!-- /.section, /#header -->

    <?php print $messages; ?>

      <div id="content" class="column"><div class="section">
        <a id="main-content"></a>
        <?php if ($tabs): ?><div class="tabs"><?php print render($tabs); ?></div><?php endif; ?>
        <?php print render($page['help']); ?>
        <?php if ($action_links): ?><ul class="action-links"><?php print render($action_links); ?></ul><?php endif; ?>
        <?php if (!$is_front && (arg(0) != 'taxonomy')) :?><div id="single_left"><?php endif;?>
        <?php print render($title_prefix); ?>
        <?php if ($title): ?><h1 class="title" id="page-title"><?php print $title; ?></h1><?php endif; ?>
        <?php print render($title_suffix); ?>
        <?php print render($page['content']); ?>
        <?php print $feed_icons; ?>
        <?php if (!$is_front && (arg(0) != 'taxonomy')) :?>
          </div><!-- /single_left -->
          <?php if ($page['sidebar']): ?>
            <div id="sidebar" class="column sidebar"><div class="section">
            <?php print render($page['sidebar']); ?>
            </div></div> <!-- /.section, /#sidebar-first -->
          <?php endif; ?>
        <?php endif;?>
      </div></div> <!-- /.section, /#content -->
    <div id="footer"><div class="section">
      <?php print render($page['footer']); ?>
    </div></div> <!-- /.section, /#footer -->

  </div> <!-- /#main_container -->
