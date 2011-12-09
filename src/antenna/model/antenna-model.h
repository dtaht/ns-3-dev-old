/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2011 CTTC
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Author: Nicola Baldo <nbaldo@cttc.es>
 */

#ifndef ANTENNA_MODEL_H
#define ANTENNA_MODEL_H


#include <ns3/object.h>
#include <ns3/mobility-model.h>
#include <ns3/spectrum-value.h>

namespace ns3 {


/** 
 * 
 * \ingroup antenna
 *
 * struct holding the azimuth and elevation angles of spherical
 * coordinates. The notation is the one used in  "Antenna Theory - Analysis
 * and Design", C.A. Balanis, Wiley, 2nd Ed., section 2.2 "Radiation pattern".
 * 
 */
struct Angles
{
  /** 
   * default constructor, will inizialize phi and theta to zero
   * 
   */
  Angles ();

  /** 
   * this constructor allows to specify phi and  theta
   * 
   * \param phi 
   * \param theta 
   * 
   */
  Angles (double phi, double theta);

  /** 
   * this constructor will initialize phi and theta by converting the
   * given 3D vector from cartesian coordinates to spherical coordinates
   * 
   * \param c the 3D vector in cartesian coordinates
   * 
   */
  Angles (Vector c);

  /**
   * the azimuth angle in radians
   * 
   */
  double phi;

  /**
   * the elevation angle in radians
   * 
   */
  double theta;
};

/** 
 * \ingroup antenna
 * 
 * \brief interface for antenna radiation pattern models
 * 
 * This class provides an interface for the definition of antenna
 * radiation pattern models. This interface is based on the use of
 * spherical coordinates, in particolar of the azimuth and elevation
 * angles. This choice is the one proposed "Antenna Theory - Analysis
 * and Design", C.A. Balanis, Wiley, 2nd Ed., see in particular
 *  section 2.2 "Radiation pattern".
 */
class AntennaModel : public Object
{
public:
  AntennaModel ();
  virtual ~AntennaModel ();

  // inherited from Object
  static TypeId GetTypeId ();

  /**       
   * 
   * \param the spherical angles at which the radiation pattern should
   * be evaluated
   * 
   * \return the gain in dB of the antenna radiation pattern at the specified angles
   */
  virtual double GetGainDb (Angles a) = 0;

  /** 
   * Set the boresight of the antenna, i.e., is orientation.
   * 
   * \param a the spherical angles specifyint the boresight of the antenna
   */
  virtual void SetBoresight (Angles a) = 0;
}


} // namespace ns3