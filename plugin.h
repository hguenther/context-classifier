/**
 * @file
 * @addtogroup plugin Plugin
 * Implements the plugin system.
 * @{
 */
#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#define _GNU_SOURCE

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "rule.h"
#include "performance.h"
#include "scheduling.h"
#include "battery.h"

/**
 * A callback type to extract a feature vector.
 * @see plugin_t
 * @param user_data The plugin-specific state information
 * @param result An array large enough to hold the resulting feature vector
 * @param ground_truth A pointer in which to store the expected classification result (in case of sensor data playback)
 * @return 0 if the vector could be fetched <0 otherwise
 */
typedef int (*feature_getter_t)(void* user_data,double* result,char** ground_truth,const int* semantics);

typedef void (*classification_cb_t)(char* class,double raw,char* ground_truth,void* user_data);

typedef void (*plugin_destructor_t)(void* user_data);

typedef void (*feature_skipper_t)(void* user_data);

typedef int (*semantics_mapper_t)(const char* term);

/**
 * A plugin handler. Holds static information about the plugin as well as plugin-specific state information.
 */
typedef struct {
  ///The name of the plugin
  char* name;
  ///Plugin-specific state information
  void* user_data;
  ///The size of the vectors this plugin generates
  int feature_vector_size;
  ///The function to extract new feature-vectors
  feature_getter_t callback;
  ///A function to clean up the plugin-specific state information
  plugin_destructor_t destructor;
  ///A function to skip a feature vector
  feature_skipper_t skipper;
  ///A function to map the semantics of a term to an array index
  semantics_mapper_t semantic_mapper;
} plugin_t;

/**
 * Clean up all resources used by the plugin.
 * @param plugin The plugin to destroy.
 */
void free_plugin(plugin_t* plugin);
/**
 * Spawn a new thread which handles the plugin.
 */
pid_t dispatch_plugin(const plugin_t* plugin,rule_list_t* rules,classification_cb_t cb,void* cb_data,int* running,const int* semantics,int scheduling);

#endif
/*
 * @}
 */
