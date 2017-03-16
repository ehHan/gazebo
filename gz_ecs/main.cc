/*
 * Copyright (C) 2017 Open Source Robotics Foundation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/

#include <iostream>
#include <cstdlib>

#include "gazebo/components/Fraction.hh"
#include "gazebo/components/Triplet.hh"
#include "gazebo/ecs_core/EntityManager.hh"
#include "gazebo/ecs_core/SystemManager.hh"
#include "gazebo/plugin/PluginLoader.hh"

// I can't imagine why someone would inherit from a system. Calling these
// private gives freedom to make changes without worring about breaking code
#include "gazebo/private/systems/DivideAndPrintResult.hh"

int main(int argc, char **argv)
{
  // ECS libraries EntityX, anax, and artemis all have a "world" class
  // that handles entity, component, and system storage. I'm avoiding making
  // one because of possible confusion with gazebo worlds

  // Something to store entities and components
  gazebo::ecs_core::EntityManager em;

  // Something to store Systems
  gazebo::ecs_core::SystemManager sm;

  // Something to deal with loading plugins
  gazebo::plugin::PluginLoader pm;

  // Add a place to search for plugins
  const char *path = std::getenv("GAZEBO_PLUGIN_PATH");
  if (nullptr != path)
    pm.AddSearchPath(std::string(path));
  else
    std::cerr << "No plugin path given" << std::endl;

  // All systems are going to require entities and components to operate on
  sm.SetEntityManager(&em);

  // First way to load a system: not using a plugin. Useful for testing
  sm.LoadSystem<gazebo::systems::DivideAndPrintResult>();

  // Second way to load a system: using a plugin.
  if (pm.LoadLibrary("AddAndPrintResult"))
  {
    std::unique_ptr<gazebo::ecs_core::System> sys;
    sys = pm.Instantiate<gazebo::ecs_core::System>(
        "::gazebo::systems::AddAndPrintResult",
        "::gazebo::ecs_core::System");
    if (!sm.LoadSystem(std::move(sys)))
    {
      std::cerr << "Faied to load plugin from library" << std::endl;
    }
  }
  else
  {
    std::cerr << "Failed to load library" << std::endl;
  }

  // Create a few entities to work with
  for (int i = 0; i < 10; i++)
  {
    // EntityX, anax, and artemis all have an Entity class that acts as a
    // convenience wrapper for world or entity manager calls. This line is
    // is different in that it returns the ID which can be used for calling
    // the EntityManager. It is less convenient, but it avoids giving the 
    // impression that an Entity is more than an ID.
    gazebo::ecs_core::Entity e = em.CreateEntity();
    
    if (e % 2 == 0)
    {
      auto fraction = em.AddComponent<gazebo::components::Fraction>(e);
      fraction->numerator = 100.0f + i;
      fraction->denominator = 1.0f + i;
    }
    if (e % 3 == 0)
    {
      // This test program knows about components, but AddComponent<>() will
      // really be called by a componentizer plugin. Gazebo won't know what
      // components are beyond their typeid hash and size.
      auto numbers = em.AddComponent<gazebo::components::Triplet>(e);
      numbers->first = e;
      numbers->second = i;
      numbers->third = 3;
    }
  }

  // Update results of EntityManager queries after adding entities
  em.Update();

  // Run all the systems once. Value chosen for time_step is unimportant for
  // this demo. In practice Update() should be called in a loop for as long
  // as the simulation is running.
  double timeStep = 0.001;
  sm.Update(timeStep);

  return 0;
}
