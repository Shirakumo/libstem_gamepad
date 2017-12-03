/*
  Copyright (c) 2014 Alex Diener
  
  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.
  
  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:
  
  1. The origin of this software must not be misrepresented; you must not
  claim that you wrote the original software. If you use this software
  in a product, an acknowledgment in the product documentation would be
  appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
  misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
  
  Alex Diener alex@ludobloom.com

  Modified by
  Nicolas Hafner <shinmera@tymoon.eu>
*/

#include "gamepad/Gamepad.h"
#include "gamepad/Gamepad_private.h"
#include <stdlib.h>

int device_map_count = 0;
int device_map_size = 0;
struct Gamepad_device_map_i **device_maps = 0;
struct Gamepad_device_map_i generic_device = {
  .buttonMap      = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  .axisMap        = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
  .axisMultiplier = {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
  .id = 0
};

uint32_t to_id(int vendorID, int productID){
  return ((vendorID & 0xFFFF) << 16) | (productID & 0xFFFF);
}

int find_device_map(int vendorID, int productID){
  if(device_maps == 0) return -1;
  
  uint32_t id = to_id(vendorID, productID);
  for(int i=0; i<device_map_count; ++i){
    if(device_maps[i]->id == id) return i;
  }
  return -1;
}

STEM_GAMEPAD_EXPORT struct Gamepad_device_map *Gamepad_deviceMap(int vendorID, int productID){
  int map_index = find_device_map(vendorID, productID);
  if(map_index != -1){
    return (struct Gamepad_device_map *)device_maps[map_index];
  }else{
    return (struct Gamepad_device_map *)&generic_device;
  }
}

STEM_GAMEPAD_EXPORT char Gamepad_setDeviceMap(int vendorID, int productID, struct Gamepad_device_map *map){
  if(map == 0){
    int map_index = find_device_map(vendorID, productID);
    if(map_index != -1){
      // WARN: this might actually fuck things up because devices
      //       might still point to the freed device map.
      free(device_maps[map_index]);
      for(int i=map_index; i<device_map_count-1; ++i){
        device_maps[i] = device_maps[i+1];
      }
      device_map_count -= 1;
    }
  }else{
    // Increase storage if necesssary
    if(device_map_size < device_map_count+1){
      struct Gamepad_device_map_i **new = (device_maps == 0)
        ? calloc(32, sizeof(struct Gamepad_device_map_i *))
        : realloc(device_maps, sizeof(struct Gamepad_device_map_i *)*(device_map_size+32));
      if(new == 0) return 0;
      
      device_maps = new;
      device_map_size += 32;
    }

    struct Gamepad_device_map_i *internal;
    int map_index = find_device_map(vendorID, productID);
    
    if(map_index != -1){
      internal = device_maps[map_index];
    }else{
      // Allocate a new storage since we don't have this map yet.
      internal = calloc(1, sizeof(struct Gamepad_device_map_i));
      if(internal == 0) return 0;
      internal->id = to_id(vendorID, productID);
      device_maps[device_map_count] = internal;
      device_map_count += 1;
    }
    
    for(int i=0; i<STEM_GAMEPAD_DEVICE_MAP_MAX; ++i){
      internal->buttonMap[i] = map->buttonMap[i];
      internal->axisMap[i] = map->axisMap[i];
      internal->axisMultiplier[i] = map->axisMultiplier[i];
    }
    
    return 1;
  }
}

void (* Gamepad_deviceAttachCallback)(struct Gamepad_device * device, void * context) = NULL;
void (* Gamepad_deviceRemoveCallback)(struct Gamepad_device * device, void * context) = NULL;
void (* Gamepad_buttonDownCallback)(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context) = NULL;
void (* Gamepad_buttonUpCallback)(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context) = NULL;
void (* Gamepad_axisMoveCallback)(struct Gamepad_device * device, unsigned int buttonID, float value, float lastValue, double timestamp, void * context) = NULL;
void * Gamepad_deviceAttachContext = NULL;
void * Gamepad_deviceRemoveContext = NULL;
void * Gamepad_buttonDownContext = NULL;
void * Gamepad_buttonUpContext = NULL;
void * Gamepad_axisMoveContext = NULL;

STEM_GAMEPAD_EXPORT void Gamepad_deviceAttachFunc(void (* callback)(struct Gamepad_device * device, void * context), void * context) {
  Gamepad_deviceAttachCallback = callback;
  Gamepad_deviceAttachContext = context;
}

STEM_GAMEPAD_EXPORT void Gamepad_deviceRemoveFunc(void (* callback)(struct Gamepad_device * device, void * context), void * context) {
  Gamepad_deviceRemoveCallback = callback;
  Gamepad_deviceRemoveContext = context;
}

STEM_GAMEPAD_EXPORT void Gamepad_buttonDownFunc(void (* callback)(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context), void * context) {
  Gamepad_buttonDownCallback = callback;
  Gamepad_buttonDownContext = context;
}

STEM_GAMEPAD_EXPORT void Gamepad_buttonUpFunc(void (* callback)(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context), void * context) {
  Gamepad_buttonUpCallback = callback;
  Gamepad_buttonUpContext = context;
}

STEM_GAMEPAD_EXPORT void Gamepad_axisMoveFunc(void (* callback)(struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context), void * context) {
  Gamepad_axisMoveCallback = callback;
  Gamepad_axisMoveContext = context;
}
