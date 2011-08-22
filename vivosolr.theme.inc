<?php

/**
 * @file
 *  Custom theme functions
 */

/**
 * Default theme function for spelling suggestions.
 */
function theme_vivosolr_suggestions($variables) {
  $output = '<div class="spelling-suggestions">';
  $output .= '<dl class="form-item"><dt><strong>' . t('Did you mean') . '</strong></dt>';
  foreach ((array) $variables['links'] as $link) {
    $output .= '<dd>' . $link . '</dd>';
  }
  $output .= '</dl></div>';
  return $output;
}


/**
 * Shows a groups of blocks for starting a search from a filter.
 */
function theme_vivosolr_browse_blocks($vars) {
  $result = "<div class='apachesolr-browse-blocks'>\n<h2>" . t('Browse available categories') . "</h2>\n";
  $result .= '<p>' . t('Pick a category to launch a search.') . "</p>\n";
  $result .= $vars['content']['#children'] . "\n</div>\n";

  return $result;
}

/**
 * Theme the highlighted snippet text for a search entry.
 *
 * @param array $vars
 *
 */
function theme_vivosolr_snippets($vars) {
  $result = '';
  if (is_array($vars['snippets']) && !empty($vars['snippets'])) {
    $result .= ' ... ';
    $result .= implode(' ... ', $vars['snippets']);
    $result .= ' ...';
  }
  return $result;
}

/**
 * Brief message to display when no results match the query.
 *
 * @see search_help()
 */
function theme_vivosolr_noresults() {
  return t('<ul>
<li>Check if your spelling is correct, or try removing filters.</li>
<li>Remove quotes around phrases to match each word individually: <em>"blue drop"</em> will match less than <em>blue drop</em>.</li>
<li>You can require or exclude terms using + and -: <em>big +blue drop</em> will require a match on <em>blue</em> while <em>big blue -drop</em> will exclude results that contain <em>drop</em>.</li>
</ul>');
}