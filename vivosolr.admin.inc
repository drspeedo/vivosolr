<?php

/**
 * @file
 *   Administrative pages
 */

/**
 * Form builder for general settings used as a menu callback.
 */
function vivosolr_settings_form($form, &$form_state) {
  $searches_enabled = variable_get('search_active_modules', array());
  $searches_default = variable_get('search_default_module', NULL);

  if ($searches_default == 'vivosolr') {
    $status = t('VIVO search is enabled and is the default.');
  }
  else if (in_array('vivosolr', $searches_enabled)) {
    $status = t('VIVO search is enabled, but is not the default.');
  }
  else {
    $status = t('VIVO search is disabled.');
  }

  $status .=  t(' To change this setting visit the !page.', array('!page' => l('search settings page', 'admin/config/search/settings')));

  $form['vivosolr_status'] = array(
    '#type' => 'item',
    '#prefix' => '<p>',
    '#markup' => $status,
    '#suffix' => '</p>',
  );

  $form['vivosolr_solr_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr server URL'),
    '#default_value' => variable_get('vivosolr_solr_url', NULL),
    '#description' => t('URL that identifies the Solr server used by VIVO. Example: http://vivo.institution.edu:8080/solr'),
    '#required' => TRUE,
  );

  $form = system_settings_form($form);

  $form['actions']['test'] = array(
    '#type' => 'submit',
    '#submit' => array('vivosolr_settings_form_test_submit'),
    '#value' => t('Test connection'),
  );

  return $form;
}

/**
 * Validation for settings form. Copied from apachesolr.module.
 */
function vivosolr_settings_form_validate($form, &$form_state) {
  $parts = parse_url($form_state['values']['vivosolr_solr_url']);
  foreach (array('scheme', 'host', 'path') as $key) {
    if (empty($parts[$key])) {
      form_set_error('url', t('The Solr server URL needs to include a !part', array('!part' => $key)));
    }
  }
  if (isset($parts['port'])) {
    // TODO: Port range should be 0-65535, but 0 crashes apachesolr
    if (!ctype_digit($parts['port']) || $parts['port'] < 1 || $parts['port'] > 65535) {
      form_set_error('port', t('The port has to be an integer between 1 and 65535.'));
    }
    else {
      // Normalize the path to have just a leading slash.
      $form_state['values']['vivosolr_solr_url'] = trim($form_state['values']['vivosolr_solr_url'], "/ \t\r\n\0\x0B");
    }
  }
}

function vivosolr_settings_form_test_submit($form, &$form_state) {
  $ping = apachesolr_server_status($form_state['values']['vivosolr_solr_url']);
  if ($ping) {
    drupal_set_message(t('Successfully contacted the VIVO Solr server.'));
  }
  else {
    drupal_set_message(t('Unable to contact the VIVO Solr server.'), 'error');
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Page callback for index statistics menu item.
 */
function vivosolr_statistics_page() {
  return '';
}