<?php
/**
 * @file
 *  
 * CKeditor Upload tab support
 */

function elfinder_editor_upload_ckeditor() {
    
  $p = elfinder_get_user_profile();

  $dest = isset($p->settings['ckeditor_upload_directory']) ? elfinder_parse_path_tokens($p->settings['ckeditor_upload_directory']) : 'private://';
  
  $tmf = $_FILES;
 
  foreach (array_keys($_FILES['upload']) as $key) {
    $tmpf['files'][$key]['upload'] = $_FILES['upload'][$key];
  }
 
  $_FILES = $tmpf;

  $file = file_save_upload('upload', array(), $dest);

  header('Content-Type: text/html');
  
  print '<script type="text/javascript">';

  if ($file) {
    print "window.parent.CKEDITOR.tools.callFunction(2, '" . file_create_url($file->uri) . "', '');";
  }
  else {
    print "window.parent.CKEDITOR.tools.callFunction(2, '', '" . t('Error uploading file!') . "');";
  }

  print '</script>';

  exit();
}