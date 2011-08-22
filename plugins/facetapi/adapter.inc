<?php

/**
 * @file
 * Classes used by the Facet API module.
 */

/**
 * Extend the Apache Solr module's adapter to do some minor overrides.
 */
class VivosolrFacetapiAdapter extends ApacheSolrFacetapiAdapter {

  /**
   * Override to change our module's admin path.
   */
  public function getPath($realm_name) {
    $base_path = 'admin/config/search/vivosolr';
    return $base_path . '/facets/' . $realm_name;
  }

  /**
   * Override to implement our custom filter-clearing flags.
   * 
   * These flags are added to the facet settings defined in hook_facetapi_facet_info().
   * Changes to the original getQueryString() method are marked with
   * the text "ADDED".
   */
  public function getQueryString(array $facet, array $values, $active) {
    $qstring = $this->params;
    $active_items = $this->getActiveItems($facet);
    
    // ADDED: The "clear all filters" flag clears out ALL active items in ALL facets.
    if (!empty($facet['clear all filters'])) {
      $qstring[$this->filterKey] = array();
    }
    
    // ADDED: The "clear filters" flag clears out other active items with the facet.
    if (!empty($facet['clear filters'])) {
      foreach ($qstring[$this->filterKey] as $i => $filter) {
        // Break apart the filter's query string to find the target field.
        list($field, $value) = explode(':', $filter, 2);
        // Remove the filter if it's from the current facet.
        if ($field == $facet['field alias']) {
          unset($qstring[$this->filterKey][$i]);
        }
      }
    }

    // Appends to qstring if inactive, removes if active.
    foreach ($values as $value) {
      if ($active && isset($active_items[$value])) {
        unset($qstring[$this->filterKey][$active_items[$value]['pos']]);
      }
      elseif (!$active) {
        $field_alias = rawurlencode($facet['field alias']);
        $qstring[$this->filterKey][] = $field_alias . ':' . $value;
      }
    }

    // Resets array keys, returns query string.
    $qstring[$this->filterKey] = array_values($qstring[$this->filterKey]);
    return array_filter($qstring);
  }
}