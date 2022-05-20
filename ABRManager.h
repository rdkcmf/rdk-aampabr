/*
 *   Copyright 2018 RDK Management
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
*/

#ifndef ABR_MANAGER_H
#define ABR_MANAGER_H

#include <vector>
#include <map>
#include <string>
#include <cstdio>


/**
 * @brief ABR Manager for HLS/DASH
 * 
 * It returns the profile index with the desired bitrate
 * base on the network condition.
 */
class ABRManager {
public:
  /**
   * @brief The profile info used
   * for ramping up/down bitrate.
   */
  struct ProfileInfo {
    /**
     * @brief Is iframe track
     */
    bool isIframeTrack;

    /**
     * @brief Bandwidth / second (Bitrate)
     */
    long bandwidthBitsPerSecond;

    /**
     * @brief Width of resolution (optional)
     */
    int width;

    /**
     * @brief Height of resolution (optional)
     */
    int height;

    /**
     * @brief period-Id of profiles (optional)
     */
    std::string periodId;

    /**
     * @brief profileIndex or PeriodIndex (optional)
     */
    int userData;
  };

  /**
   * @brief Logger type
   */
  typedef int (*LoggerFuncType)(const char* fmt, ...);

  /**
   * @brief Persist Network Bandwidth 
   */
  static long mPersistBandwidth;

  /**
   * @brief Persist Network Bandwidth Updated Time
   */

  static long long mPersistBandwidthUpdatedTime;
public:
  /**
   * @brief Constructor of ABRManager
   */
  ABRManager();

  /**
   * @brief Get initial profile index, choose the medium profile or
   * the profile whose bitrate >= the default bitrate.
   * 
   * @param chooseMediumProfile Boolean flag, true means
   * to choose the medium profile, otherwise to choose the profile whose
   * bitrate >= the default bitrate.
   * @param periodId empty string by default, Period-Id of the profiles
   * added to ABR map
   * 
   * @return The initial profile index 
   */
  int getInitialProfileIndex(bool chooseMediumProfile, const std::string& periodId= std::string());

  /**
   * @brief Update the lowest / desired profile index
   *    by the profile info. 
   */
  void updateProfile();

  /**
   * @brief According to the given bandwidth, return the best matched
   * profile index.
   * @param bandWidth  The given bandwidth
   * @return the best matched profile index
   */
  int getBestMatchedProfileIndexByBandWidth(int bandwidth);

  /**
   * @brief Ramp down the profile one step to get the profile index of a lower bitrate.
   *
   * @param currentProfileIndex The current profile index
   * @param periodId empty string by default, Period-Id of profiles
   *
   * @return the profile index of a lower bitrate (one step)
   */
  int getRampedDownProfileIndex(int currentProfileIndex, const std::string& periodId= std::string());

  /**
   * @brief Ramp up the profile one step to get the profile index of a upper bitrate.
   *
   * @param currentProfileIndex The current profile index
   * @param periodId empty string by default, Period-Id of profiles
   *
   * @return the profile index of a upper bitrate (one step)
   */
  int getRampedUpProfileIndex(int currentProfileIndex, const std::string& periodId= std::string());

  /**
   * @brief Check if the bitrate of currentProfileIndex reaches to the lowest.
   *
   * @param currentProfileIndex The current profile index
   * @param periodId empty string by default, Period-Id of profiles
   *
   * @return True means it reaches to the lowest, otherwise, it doesn't.
   */
  bool isProfileIndexBitrateLowest(int currentProfileIndex, const std::string& periodId= std::string());

  /**
   * @brief Do ABR by ramping bitrate up/down according to the current
   * network status. Returns the profile index with the bitrate matched with
   * the current bitrate.
   * 
   * @param currentProfileIndex The current profile index
   * @param currentBandwidth The current band width
   * @param networkBandwidth The current available bandwidth (network bandwidth)
   * @param nwConsistencyCnt Network consistency count, used for bitrate ramping up/down
   * @param periodId empty string by default, Period-Id of profiles
   * @return int Profile index
   */
  int getProfileIndexByBitrateRampUpOrDown(int currentProfileIndex, long currentBandwidth, long networkBandwidth, int nwConsistencyCnt = DEFAULT_ABR_NW_CONSISTENCY_COUNT, const std::string& periodId= std::string());

  /**
   * @brief Get bandwidth of profile
   *
   * @param profileIndex The profile index
   * @return long bandwidth of the profile
   */
  long getBandwidthOfProfile(int profileIndex);

  /**
   * @brief Get profile of bandwidth
   * 
   * @param bandwidth The bandwidth
   * @return int index of the bandwidth
   */
  int getProfileOfBandwidth(long bandwidth);

  /**
   * @brief Get the index of max bandwidth
   * 
   * @param periodId empty string by default, Period-Id of profiles
   *
   * @return int index of the max bandwidth
   */
  int getMaxBandwidthProfile(const std::string& periodId = std::string());
public:
  // Getters/Setters
  /**
   * @brief Get the number of profiles
   * 
   * @return The number of profiles 
   */
  int getProfileCount() const;

  /**
   * @brief Set the default init bitrate
   * 
   * @param defaultInitBitrate Default init bitrate
   */
  void setDefaultInitBitrate(long defaultInitBitrate);

  /**
   * @brief Get the lowest iframe profile index.
   * 
   * @return the lowest iframe profile index. 
   */
  int getLowestIframeProfile() const;

  /**
   * @brief Get the desired iframe profile index.
   * 
   * @return the desired iframe profile index. 
   */
  int getDesiredIframeProfile() const;

  /**
   * @brief Add new profile info into the manager
   * @param profile The profile info
   */
  void addProfile(ProfileInfo profile);

  /**
   * @brief Clear profiles
   */
  void clearProfiles();

  /**
   * @brief Set logger function
   * 
   * The logger function must be in the signature int (const char*, ...)
   * 
   * @param logger The logger function
   */
  static void setLogger(LoggerFuncType logger);

  /**
   * @brief Disable logger, then no logger output.
   */
  static void disableLogger();

  /**
   * @brief Configure the simulator log file directory.
   */
  void setLogDirectory(char driveName);

  /**
   * @brief Set the default iframe bitrate
   * 
   * @param defaultIframeBitrate Default iframe bitrate
   */
  void setDefaultIframeBitrate(long defaultIframeBitrate);
   /**
    * @brief Get UserData of profile
    *
    * @param profileIndex The profile index
    * @return int userdata / period index
    */
   int getUserDataOfProfile(int profileIndex);
   /**
    * @brief Set the Persist Network Bandwidth
    *
    * @param network bitrate
    */
   static void setPersistBandwidth(long bitrate){mPersistBandwidth = bitrate;}
   /**
    * @brief Get Persisted Network Bandwidth
    *
    * @return  bandwidth
    */
   static long getPersistBandwidth() { return mPersistBandwidth;}
private:
  /**
   * @brief A list of available profiles.
   */
  std::vector<ProfileInfo> mProfiles;

  /**
   * @brief A sorted list of profiles with periodId.
   * Populate the container with sorted order of BW (Bandwidth) vs its index under each periodId
   */
  std::map<std::string, std::map<long,int>> mSortedBWProfileList;

  /**
   * @brief Define type: iterator of SortedBWProfileListIter 
   */
  typedef std::map<long, int>::iterator SortedBWProfileListIter;

  /**
   * @brief Define type: reverse iterator of SortedBWProfileListIter 
   */
  typedef std::map<long, int>::reverse_iterator SortedBWProfileListRevIter;

  /**
   * @brief Lowest iframe Profile index
   */ 
  int mLowestIframeProfile;

  /**
   * @brief Desired iframe Profile index
   */
  int mDesiredIframeProfile;

  /**
   * @brief Default initialization bitrate
   */
  long mDefaultInitBitrate;


  /**
   * @brief The number of ABR profiles that ramping up
   */
  int mAbrProfileChangeUpCount;
  
  /**
   * @brief The number of ABR profiles that ramping down
   */
  int mAbrProfileChangeDownCount;

  /**
   * @brief Logger function pointer
   */
  static LoggerFuncType sLogger;

  /**
   * @brief Default iframe bitrate
   */
  long mDefaultIframeBitrate;

public:
  /**
   * @brief Invalid profile index
   */
  static const int INVALID_PROFILE = -1;

private:
  /**
   * @brief Default init bitrate value.
   */
  static const int DEFAULT_BITRATE = 1000000;

  /**
   * @brief The width of 4K video
   */
  static const int WIDTH_4K = 1920;

  /**
   * @brief The height of 4K video
   */
  static const int HEIGHT_4K = 1080;

  /**
   * @brief The default value of the network consistency count.
   * If the profile change exceeds this value, the ABR will be performed.
   * Used when bitrate ramping up/down
   */
  static const int DEFAULT_ABR_NW_CONSISTENCY_COUNT = 2;
};

#endif
