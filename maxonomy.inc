<?php

class maxonomy {

	public static $preferred_language_locale = 'en_US';

	public static function get_implicit_tags($object) {
		// i have this thiing, and it may or may not have any tags on it. please get me a list of all teh tags on it, and all the tags on those tags, and the tags on those tags, etc.
	}

	public static function get_direct_tags($object) {
		// i have this thiing, and it may or may not have any tags on it. please get me a list of all teh tags directly on it.
	}

	public static function add_tags($addee, $tags) {
		// add these tags to this thing.
	}

	public static function remove_tags($thing, $tags = 'clear all') {
		// remove these/all tags from this thing
	}

	public static function get_these_things_that_have_these_tags($type, $tags) {
		// get me a list of ids? or full objects? of this type (class name) with these tags.
	}

	public static function set_preferred_language_locale($language_locale = NULL) {
		// maybe you want spanish tags, if they exist.
	}

}


class tag {
	public $en_US; // english (United-States) text of the tag varchar 60
	public $es_LA; // spanish (Latin-American) text of the tag
	public $ja; // japanese text of the tag
	// public $zh; // chinese text of the tag - add or comment out as many of these as you want, but make a field in the table with the same name, and make sure you keep at least one.
}