<?php

// limitation: must use ints as ids for anything you want to tag.
// in the class definition, add the consts.
// hmmm you can't always edit class files...... but then, why would you want to classify a class you didn't make?


class maxonomy {

	public static $preferred_language_locale = 'en_US';

	public static function get_implicit_tags($object) {
		// i have this thiing, and it may or may not have any tags on it. please get me a list of all teh tags on it, and all the tags on those tags, and the tags on those tags, etc.
	}

	public static function get_direct_tags($object) {
		// i have this thiing, and it may or may not have any tags on it. please get me a list of all teh tags directly on it.
	}

	public static function add_tags($tagee, $tags) {
		// need an item_class_id, which reflects what it is - event, user, tag, etc.
		// for now, need an item_id as well, which is the primary key value for that thing. without it, the tags would apply to the class itself. which may be what you want.
		// we'll be adding a record for each tag id.
		// add these tags to this thing.

		if(is_object($tagee)) {
			if( ! defined(get_class($tagee) . '::MAXONOMY_ID') or ! defined(get_class($tagee) . '::MAXONOMY_PRIMARY_KEY_PROPERTY_NAME')) throw new Exception(get_class($tagee) . 's are not prepared for maxonification. please add the required class constants, or provide the values as arguments.');
			$item_class_id = (int) $tagee::MAXONOMY_ID;
			$item_id = (int) $tagee->{$tagee::MAXONOMY_PRIMARY_KEY_PROPERTY_NAME};
		/*} elseif(is_array($tagee)) {
			$item_class_id = (int) array_shift($tagee);
			$item_id = (int) array_shift($tagee);
			// todo make sure we got good ints.*/
		} else {
			throw new Exception('bad tagee args.');
		}

		// if we don't have item class and id, we shoul dhave thrown exception by now.
		// so let's work on the list of tag_ids.
		if(is_scalar($tags)) $tags = array($tags);

		foreach($tags as $key => $tag) {
			if(is_scalar($tag)) {
				// create a tag obj
				$tags[$key] = new tag($tag);
			} elseif(is_object($tag) and get_class($tag) == 'tag') {
				// validate?
			} else {
				throw new Exception('bad tags.');
			}
		}

		m::death($tags, 'class id:' . $item_class_id . ' | item id:' . $item_id);


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
	public $en; // english (United-States) text of the tag varchar 60
	public $en_GB; // english (British) text of the tag varchar 60
	public $es; // spanish (Latin-American) text of the tag
	public $ja; // japanese text of the tag
	// public $zh; // chinese text of the tag - add or comment out as many of these as you want, but make a field in the table with the same name, and make sure you keep at least one.

	private $languages = array('en', 'es', 'ja', 'en_GB'); // same as the props above.

	public $tag_id;

	private $loaded = false;
	private $saved = false;
	private $error = false;
	private $error_messages = array();

	const SCEMA = 'test';
	const TABLE_NAME = 'tag';
	const MAXONOMY_ID = 1;
	const MAXONOMY_PRIMARY_KEY_PROPERTY_NAME = 'tag_id';

	public function __construct($en = NULL, $es = NULL, $ja = NULL, $dialects = NULL) {
		foreach($this->languages as $lang) if(isset($$lang)) $$lang = trim($$lang);

		if(empty($en) and empty($es) and empty($ja) and (empty($dialects) or ! is_array($dialects))) throw new Exception('cannot instantiate empty tag.');
		if(empty($en) and empty($es) and empty($ja)) throw new Exception('todo: set the dialect field, then also set parent language. actually, do this if the parent language is empty, not all the languages.');

		foreach($this->languages as $lang) if(isset($$lang)) $this->$lang = $$lang;
	}

	public static function get_tag_by_id($tag_id = NULL) {
		if(empty($tag_id) or ! is_numeric($tag_id) or ! $tag_id) {
			trigger_error('bad tag id', E_USER_WARNING);
			return false;
		}

		$tag = new tag('temp'); // temp because you can't instantiate by id... need something to fill it
		$tag->tag_id = (int) $tag_id;
		$tag->load();

		if($tag->error) return false; // if it couldn't load, i don't want to return 'temp'

		return $tag;
	}

	private function load() { // arbitrary privacy
		$this->loaded = false;

		if( ! isset($this->tag_id) or ! is_numeric($this->tag_id) or ! $this->tag_id) {
			$this->error = true;
			$this->error_messages[] = 'bad tag id for load.';
			return false;
		}

		// run queries
		$this->en = 'whatevs';
		$this->loaded = true;
	}
}