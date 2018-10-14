// Copyright (c) 2017 Computer Vision Center (CVC) at the Universitat Autonoma
// de Barcelona (UAB).
//
// This work is licensed under the terms of the MIT license.
// For a copy, see <https://opensource.org/licenses/MIT>.

#pragma once

#include "Geometry.h"
#include "RoadInfo.h"

#include <cstdio>
#include <memory>

namespace carla {
namespace road {
namespace element {

  using id_type = size_t;

  class RoadSegmentDefinition {
  public:

    RoadSegmentDefinition(RoadSegmentDefinition &&rsd)
      : _id(rsd._id),
        _predecessor_id(std::move(rsd._predecessor_id)),
        _geom(std::move(rsd._geom)),
        _info(std::move(rsd._info)) {}

    RoadSegmentDefinition(id_type id) {
      assert(id > 0);
      _id = id;
    }

    const id_type &GetId() const {
      return _id;
    }

    void AddPredecessorID(const id_type &id) {
      _predecessor_id.emplace_back(id);
    }

    void AddSuccessorID(const id_type &id) {
      _successor_id.emplace_back(id);
    }

    // usage MakeGeometry<GeometryArc>(len, st_pos_offs, head, st_pos, curv)
    template <typename T, typename ... Args>
    void MakeGeometry(Args && ... args) {
      _geom.emplace_back(std::make_unique<T>(std::forward<Args>(args) ...));
    }

    // usage MakeInfo<SpeedLimit>(30.0)
    template <typename T, typename ... Args>
    T *MakeInfo(Args && ... args) {
      _info.emplace_back(std::make_unique<T>(std::forward<Args>(args) ...));
      return dynamic_cast<T *>(_info.back().get());
    }

    const std::vector<id_type> &GetPredecessorID() const {
      return _predecessor_id;
    }
    const std::vector<id_type> &GetSuccessorID() const {
      return _successor_id;
    }
    const std::vector<std::unique_ptr<Geometry>> &GetGeometry() const {
      return _geom;
    }
    const std::vector<std::unique_ptr<RoadInfo>> &GetInfo() const {
      return _info;
    }

  private:

    friend class RoadSegment;
    id_type _id;
    std::vector<id_type> _predecessor_id;
    std::vector<id_type> _successor_id;
    std::vector<std::unique_ptr<Geometry>> _geom;
    std::vector<std::unique_ptr<RoadInfo>> _info;
  };

} // namespace element
} // namespace road
} // namespace carla