<?php

/**
 * @file
 *  Plugins and definitions for Facet API
 */

/**
 * Implements hook_facetapi_widgets().
 */
function vivosolr_facetapi_widgets() {
  return array(
    'vivosolr_visual_links' => array(
      'handler' => array(
        'label' => t('Links with visual counts'),
        'class' => 'VivosolrVisualLinksWidget',
      ),
    ),
  );
}

/**
 * Implements hook_facetapi_adapters().
 */
function vivosolr_facetapi_adapters() {
  return array(
    'vivo_adapter' => array(
      'handler' => array(
        'class' => 'VivosolrFacetapiAdapter',
      ),
    ),
  );
}

/**
 * Implements hook_facetapi_query_types().
 */
function vivosolr_facetapi_query_types() {
  return array(
    'vivo_query' => array(
      'handler' => array(
        'class' => 'VivosolrFacetapiQuery',
        'adapter' => 'vivo_adapter',
      ),
    ),
  );
}

/**
 * Implements hook_facetapi_searcher_info().
 */
function vivosolr_facetapi_searcher_info() {
  return array(
    'vivosolr@default' => array(
      'label' => 'VIVO Search',
      'adapter' => 'vivo_adapter',
      'type' => 'vivo',
      'path' => 'admin/config/search/vivosolr',
      'supports facet mincount' => TRUE,
    ),
  );
}

/**
 * Implements hook_facetapi_facet_info().
 *
 * Defines our custom VIVO facets.
 */
function vivosolr_facetapi_facet_info(array $searcher_info) {
  $facets = array();

  if ('vivo_adapter' == $searcher_info['adapter']) {
    $facets['classgroup'] = array(
      'label' => t('Classgroup'),
      'description' => t('Filter by classgroup.'),
      'field' => VIVOSOLR_FIELD_CLASSGROUP,
      'query type' => 'vivo_query',
      'clear filters' => TRUE,
      // 'clear all filters' => TRUE,
      // 'exclude' => array(VIVOSOLR_FIELD_CLASSGROUP, VIVOSOLR_FIELD_CLASS, VIVOSOLR_FIELD_INSTITUTION),
      'facet mincount allowed' => TRUE,
    );
    $facets['type'] = array(
      'label' => t('Type'),
      'description' => t('Filter by type.'),
      'field' => VIVOSOLR_FIELD_CLASS,
      'query type' => 'vivo_query',
      'clear filters' => TRUE,
      'facet mincount allowed' => TRUE,
    );
    $facets['institution'] = array(
      'label' => t('Institution'),
      'description' => t('Filter by institution.'),
      'field' => VIVOSOLR_FIELD_INSTITUTION,
      'query type' => 'vivo_query',
      'show all' => TRUE,
      'clear filters' => TRUE,
      'clear all filters' => TRUE,
      'facet mincount allowed' => TRUE,
    );
  }

  return $facets;
}
