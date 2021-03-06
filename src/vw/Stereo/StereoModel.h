// __BEGIN_LICENSE__
//  Copyright (c) 2006-2013, United States Government as represented by the
//  Administrator of the National Aeronautics and Space Administration. All
//  rights reserved.
//
//  The NASA Vision Workbench is licensed under the Apache License,
//  Version 2.0 (the "License"); you may not use this file except in
//  compliance with the License. You may obtain a copy of the License at
//  http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing, software
//  distributed under the License is distributed on an "AS IS" BASIS,
//  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//  See the License for the specific language governing permissions and
//  limitations under the License.
// __END_LICENSE__


#ifndef __VW_STEREO_STEREOMODEL_H__
#define __VW_STEREO_STEREOMODEL_H__

#include <vw/Math/Vector.h>

namespace vw {

  template <class PixelT> class ImageView;
  template <class PixelT> struct PixelMask;

  // forward declaration
  namespace camera {
    class CameraModel;
  }

namespace stereo {

  class StereoModel {

  public:

    //------------------------------------------------------------------
    // Constructors / Destructors
    //------------------------------------------------------------------
    StereoModel(camera::CameraModel const* camera_model1,
                camera::CameraModel const* camera_model2,
                bool least_squares_refine = false);

    //------------------------------------------------------------------
    // Public Methods
    //------------------------------------------------------------------

    /// Apply a stereo model to a disparity map to produce an image of
    /// XYZ points.  Missing pixels in the disparity map will result
    /// in zero vector pixels in the point image.
    ///
    /// Users really shouldn't use this method, the ideal method is
    /// the 'stereo_triangulate' in StereoView.h.
    ImageView<Vector3> operator()(ImageView<PixelMask<Vector2f> > const& disparity_map,
                                  ImageView<double> &error ) const;

    /// Apply a stereo model to a single pair of image coordinates.
    /// Returns an xyz point.  The error is set to -1 if the rays were
    /// parallel or divergent, otherwise it returns the 2-norm of the
    /// distance between the rays at their nearest point of
    /// intersection.
    virtual Vector3 operator()(Vector2 const& pix1, Vector2 const& pix2, Vector3& errorVec ) const;
    virtual Vector3 operator()(Vector2 const& pix1, Vector2 const& pix2, double& error ) const;

    /// Returns the dot product of the two rays emanating from camera
    /// 1 and camera 2 through pix1 and pix2 respectively.  This can
    /// effectively be interpreted as the angle (in radians) between
    /// the two rays, and it is a useful test for checking when the
    /// two rays are close to parallel.
    double convergence_angle(Vector2 const& pix1, Vector2 const& pix2) const;

  protected:

    const camera::CameraModel *m_camera1, *m_camera2;
    bool m_least_squares;

    //------------------------------------------------------------------
    // Protected Methods
    //------------------------------------------------------------------

    /// Return the 2-norm of the error vector ( the vector from the
    /// closest point of intersectio of A to the closest point of
    /// intersection of B ), or -1 if the rays are parallel or
    /// divergent.
    Vector3 triangulate_point(Vector3 const& point1,
                              Vector3 const& vec1,
                              Vector3 const& point2,
                              Vector3 const& vec2,
                              Vector3& errorVec) const;

    bool are_nearly_parallel(Vector3 const& vec1, Vector3 const& vec2) const;

    void refine_point( Vector2 const& pix1,
                       Vector2 const& pix2,
                       Vector3& point ) const;
  };

}}      // namespace vw::stereo

#endif  // __VW_STEREO_STEREOMODEL_H__

