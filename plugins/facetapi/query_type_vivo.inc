<?php

/**
 * @file
 * VIVO query plugin for the vivosolr adapter.
 */

/**
 * Plugin used for general vivo queries. Based on Apache Solr module's "term" query type.
 */
class VivosolrFacetapiQuery extends FacetapiQueryType implements FacetapiQueryTypeInterface {

  /**
   * Override to specify our query type.
   */
  static public function getType() {
    return 'vivo_query';
  }

  /**
   * Overriding to implement additional tagging/excluding, plus quoted filter values.
   */
  public function execute($query) {
    $settings = $this->adapter->getFacet($this->facet)->getSettings();
    // Adds the operator parameter.
    $operator = $settings->settings['operator'];
    
    // Our custom exclusion flags.
    $exclude_tags = (FACETAPI_OPERATOR_OR != $operator) ? '' : $this->facet['field'];
    $exclude_tags .= isset($this->facet['exclude']) ? ',' . implode(',', $this->facet['exclude']) : '';
    $query->addParam('facet.field', "{!ex=$exclude_tags}" . $this->facet['field']);

    if (!empty($settings->settings['facet_missing'])) {
      $query->addParam('f.' . $this->facet['field'] . '.facet.missing', 'true');
    }
    // Adds "hard limit" parameter to prevent too many return values.
    $limit = empty($settings->settings['hard_limit']) ? 20 : (int) $settings->settings['hard_limit'];
    $query->addParam('f.' . $this->facet['field'] . '.facet.limit', $limit);

    // Adds "facet mincount" parameter to limit the number of facets.
    if (isset($settings->settings['facet_mincount'])) {
      $count = $settings->settings['facet_mincount'];
      $query->addParam('f.' . $this->facet['field'] . '.facet.mincount', $count);
    }

    $active = $this->adapter->getActiveItems($this->facet);
    
    // Adds filter based on the operator.
    if (FACETAPI_OPERATOR_OR != $operator) {
      foreach ($active as $value => $item) {
        // Handle facet missing:
        if ($value == '_empty_' && !empty($settings->settings['facet_missing'])) {
          $query->addFilter($this->facet['field'], '[* TO *]', TRUE);
        }
        else {
          // Needed quotes!
          $value = "\"$value\"";
          $query->addFilter($this->facet['field'], $value);
        }
      }
    }
    else {
      // OR facet.
      $local = "tag={$this->facet['field']}";
      $values = array_keys($active);
      // Needed quotes!
      foreach ($values as &$value) {
        $value = "\"$value\"";
      }
      if ($values) {
        $query->addFilter($this->facet['field'], '(' . implode(' OR ', $values) . ')', FALSE, $local);
      }
    }
  }

  /**
   * Overriding to allow 0 counts.
   */
  public function build() {
    $build = array();
    if ($response = apachesolr_static_response_cache($this->adapter->getSearcher())) {
      if (isset($response->facet_counts->facet_fields->{$this->facet['field']})) {
        $values = (array) $response->facet_counts->facet_fields->{$this->facet['field']};
        foreach ($values as $value => $count) {
          // CHANGED: The original code ignored zero counts here.
          $build[$value] = array('#count' => $count);
        }
      }
    }
    return $build;
  }
}
