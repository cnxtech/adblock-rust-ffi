#ifndef ADBLOCK_RUST_FFI_H
#define ADBLOCK_RUST_FFI_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct C_Engine C_Engine;

/**
 * A set of cosmetic filtering resources specific to a particular hostname, including a base
 * stylesheet to inject, a set of script injections to add to the page, and a set of exceptions
 * that can be provided when incrementally querying later selectors by class or id.
 */
typedef struct {
  const char *stylesheet;
  char *const *exceptions;
  size_t exceptions_len;
  char *const *script_injections;
  size_t script_injections_len;
} C_HostnameResources;

typedef struct {
  const char *uuid;
  const char *url;
  const char *title;
  const char *lang;
  const char *lang2;
  const char *lang3;
  const char *support_url;
  const char *component_id;
  const char *base64_public_key;
} C_FList;

/**
 * Destroy a `*c_char` once you are done with it.
 */
void c_char_buffer_destroy(char *s);

void engine_add_filter(C_Engine *engine, const char *filter);

/**
 * Adds a resource to the engine by name
 */
void engine_add_resource(C_Engine *engine,
                         const char *key,
                         const char *content_type,
                         const char *data);

/**
 * Adds a list of resources in uBlock resources format
 */
void engine_add_resources(C_Engine *engine, const char *resources);

/**
 * Adds a tag to the engine for consideration
 */
void engine_add_tag(C_Engine *engine, const char *tag);

/**
 * Returns a stylesheet containing all generic cosmetic rules that begin with any of the provided class and id selectors
 * The leading '.' or '#' character should not be provided
 */
const char *engine_class_id_stylesheet(C_Engine *engine,
                                       const char *const *classes,
                                       size_t classes_size,
                                       const char *const *ids,
                                       size_t ids_size,
                                       const char *const *exceptions,
                                       size_t exceptions_size);

/**
 * Create a new `Engine`.
 */
C_Engine *engine_create(const char *rules);

/**
 * Deserializes a previously serialized data file list.
 */
bool engine_deserialize(C_Engine *engine, const char *data, size_t data_size);

/**
 * Destroy a `Engine` once you are done with it.
 */
void engine_destroy(C_Engine *engine);

/**
 * Returns a set of cosmetic filtering resources specific to the given hostname.
 */
C_HostnameResources engine_hostname_cosmetic_resources(C_Engine *engine, const char *hostname);

/**
 * Checks if a `url` matches for the specified `Engine` within the context.
 */
bool engine_match(C_Engine *engine,
                  const char *url,
                  const char *host,
                  const char *tab_host,
                  bool third_party,
                  const char *resource_type,
                  bool *explicit_cancel,
                  bool *saved_from_exception,
                  char **redirect);

/**
 * Removes a tag to the engine for consideration
 */
void engine_remove_tag(C_Engine *engine, const char *tag);

/**
 * Checks if a tag exists in the engine
 */
bool engine_tag_exists(C_Engine *engine, const char *tag);

/**
 * Get the specific default list size
 */
C_FList filter_list_get(const char *category, size_t i);

/**
 * Get the default list size. `category` must be one of "regions" or "default"
 */
size_t filter_list_size(const char *category);

#endif /* ADBLOCK_RUST_FFI_H */
