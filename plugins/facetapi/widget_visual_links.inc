<?php

/**
 * @file
 *
 */

/**
 * Widget that renders facets as a list of clickable links.
 *
 * The actual rendering happens in a facetapi theme function. 
 * What we're doing here is adding the result count and 
 * max count to the render array.
 */
class VivosolrVisualLinksWidget extends FacetapiWidgetLinks {

  /**
   * Overrides constructor to reset the key.
   *
   * @param array $realm
   *   The realm being rendered.
   * @param array $settings
   *   The realm settings.
   * @param FacetapiFacet $facet
   *   The facet object.
   */
  public function __construct($id, array $realm, FacetapiFacet $facet, stdClass $settings) {
    parent::__construct($id, $realm, $facet, $settings);
  }

  /**
   * Recursive function that converts the render array into an array that can be
   * passed to theme_item_list().
   *
   * @param array $build
   *   The facet's render array.
   *
   * @return array
   *   The "items" parameter for theme_item_list().
   */
  function buildListItems($build) {
    $counts = array();
    foreach ($build as $value => $item) {
      $counts[] = $item['#count'];
    }
    $max = max($counts);

    $items = array();
    foreach ($build as $value => $item) {
      $row = array('class' => array());

      // Initializes variables passed to theme hook.
      $variables = array(
        'text' => $item['#markup'],
        'path' => $_GET['q'],
        'count' => $item['#count'],
        // *** These next three are custom values ***
        'max' => $max,
        'indexed_value' => $item['#indexed_value'],
        'facet' => $this->facet['field'],

        'options' => array(
          'attributes' => array('class' => $this->getItemClasses()),
          'html' => TRUE,
          'query' => $item['#query'],
        ),
      );

      // We don't display children unless the parent is clicked.
      if (empty($item['#item_children'])) {
        $row['class'][] = 'leaf';
      }
      else {
        if ($item['#active']) {
          $row['class'][] = 'expanded';
          $row['children'] = $this->buildListItems($item['#item_children']);
        }
        else {
          $row['class'][] = 'collapsed';
        }
      }

      // Gets theme hook, adds last minute classes.
      $class = ($item['#active']) ? 'facetapi-active' : 'facetapi-inactive';
      $variables['options']['attributes']['class'][] = $class;

      // Themes the link, adds row to items.
      $row['data'] = theme($item['#theme'], $variables);
      $row['class'] = array($class);
      $items[] = $row;
    }

    return $items;
  }
}
