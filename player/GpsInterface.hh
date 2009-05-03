/*
 *  Gazebo - Outdoor Multi-Robot Simulator
 *  Copyright (C) 2003  
 *     Nate Koenig & Andrew Howard
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 */
/* Desc: Gps Interface for Player
 * Author: Glenn Laguna
 * Date: 20 June 2006
 * CVS: $Id$
 */

#ifndef GPSINTERFACE_HH
#define GPSINTERFACE_HH

#include "GazeboInterface.hh"

namespace boost
{
  class recursive_mutex;
}


// Forward declarations
typedef struct gz_gps gz_gps_t;

/// \brief GPS interface
class GpsInterface : public GazeboInterface
{
  /// @brief Constructor
  public: GpsInterface(player_devaddr_t addr, GazeboDriver *driver,
              ConfigFile *cf, int section);

  /// @brief Destructor
  public: virtual ~GpsInterface();

  /// @brief Handle all messages. This is called from GazeboDriver
  public: virtual int ProcessMessage(QueuePointer &respQueue, 
              player_msghdr_t *hdr, void *data);

  /// @brief Update this interface, publish new info.
  public: virtual void Update(); 

  /// @brief Open a GPS interface when a subscription is received.  \
  /// This is called fromGazeboDriver::Subscribe
  public: virtual void Subscribe(); 

  /// @brief Close a GPS interface.  This is called from \
  /// GazeboDriver::Unsubscribe
  public: virtual void Unsubscribe();

  private: gz_gps_t *iface;

  /// @brief Gazebo id.  This needs to match and ID in a Gazebo WorldFile
  private: char *gz_id;

  /// @brief Timestamp on last data update
  private: double datatime;

  private: static boost::recursive_mutex *mutex;
};
#endif

