/*************************************************************************/
/*  gi_probe.cpp                                                         */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2020 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2020 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "gi_probe.h"

#include "core/os/os.h"

#include "core/method_bind_ext.gen.inc"
#include "mesh_instance.h"
#include "voxelizer.h"

void GIProbeData::_set_data(const Dictionary &p_data) {
	ERR_FAIL_COND(!p_data.has("bounds"));
	ERR_FAIL_COND(!p_data.has("octree_size"));
	ERR_FAIL_COND(!p_data.has("octree_cells"));
	ERR_FAIL_COND(!p_data.has("octree_data"));
	ERR_FAIL_COND(!p_data.has("level_counts"));
	ERR_FAIL_COND(!p_data.has("to_cell_xform"));

	AABB bounds = p_data["bounds"];
	Vector3 octree_size = p_data["octree_size"];
	PoolVector<uint8_t> octree_cells = p_data["octree_cells"];
	PoolVector<uint8_t> octree_data = p_data["octree_data"];
	PoolVector<int> octree_levels = p_data["level_counts"];
	Transform to_cell_xform = p_data["to_cell_xform"];

	allocate(to_cell_xform, bounds, octree_size, octree_cells, octree_data, octree_levels);
}

Dictionary GIProbeData::_get_data() const {
	Dictionary d;
	d["bounds"] = get_bounds();
	d["octree_size"] = get_octree_size();
	d["octree_cells"] = get_octree_cells();
	d["octree_data"] = get_data_cells();
	d["level_counts"] = get_level_counts();
	d["to_cell_xform"] = get_to_cell_xform();
	return d;
}

void GIProbeData::allocate(const Transform &p_to_cell_xform, const AABB &p_aabb, const Vector3 &p_octree_size, const PoolVector<uint8_t> &p_octree_cells, const PoolVector<uint8_t> &p_data_cells, const PoolVector<int> &p_level_counts) {
	VS::get_singleton()->gi_probe_allocate(probe, p_to_cell_xform, p_aabb, p_octree_size, p_octree_cells, p_data_cells, p_level_counts);
	bounds = p_aabb;
	to_cell_xform = p_to_cell_xform;
	octree_size = p_octree_size;
}

AABB GIProbeData::get_bounds() const {
	return bounds;
}
Vector3 GIProbeData::get_octree_size() const {
	return octree_size;
}
PoolVector<uint8_t> GIProbeData::get_octree_cells() const {
	return VS::get_singleton()->gi_probe_get_octree_cells(probe);
}
PoolVector<uint8_t> GIProbeData::get_data_cells() const {
	return VS::get_singleton()->gi_probe_get_data_cells(probe);
}
PoolVector<int> GIProbeData::get_level_counts() const {
	return VS::get_singleton()->gi_probe_get_level_counts(probe);
}
Transform GIProbeData::get_to_cell_xform() const {
	return to_cell_xform;
}

void GIProbeData::set_dynamic_range(float p_range) {
	VS::get_singleton()->gi_probe_set_dynamic_range(probe, p_range);
	dynamic_range = p_range;
}

float GIProbeData::get_dynamic_range() const {
	return dynamic_range;
}

void GIProbeData::set_propagation(float p_propagation) {
	VS::get_singleton()->gi_probe_set_propagation(probe, p_propagation);
	propagation = p_propagation;
}

float GIProbeData::get_propagation() const {
	return propagation;
}

void GIProbeData::set_anisotropy_strength(float p_anisotropy_strength) {
	VS::get_singleton()->gi_probe_set_anisotropy_strength(probe, p_anisotropy_strength);
	anisotropy_strength = p_anisotropy_strength;
}

float GIProbeData::get_anisotropy_strength() const {
	return anisotropy_strength;
}

void GIProbeData::set_energy(float p_energy) {
	VS::get_singleton()->gi_probe_set_energy(probe, p_energy);
	energy = p_energy;
}

float GIProbeData::get_energy() const {
	return energy;
}

void GIProbeData::set_bias(float p_bias) {
	VS::get_singleton()->gi_probe_set_bias(probe, p_bias);
	bias = p_bias;
}

float GIProbeData::get_bias() const {
	return bias;
}

void GIProbeData::set_normal_bias(float p_normal_bias) {
	VS::get_singleton()->gi_probe_set_normal_bias(probe, p_normal_bias);
	normal_bias = p_normal_bias;
}

float GIProbeData::get_normal_bias() const {
	return normal_bias;
}

void GIProbeData::set_interior(bool p_enable) {
	VS::get_singleton()->gi_probe_set_interior(probe, p_enable);
	interior = p_enable;
}

bool GIProbeData::is_interior() const {
	return interior;
}

void GIProbeData::set_use_two_bounces(bool p_enable) {
	VS::get_singleton()->gi_probe_set_use_two_bounces(probe, p_enable);
	use_two_bounces = p_enable;
}

bool GIProbeData::is_using_two_bounces() const {
	return use_two_bounces;
}

RID GIProbeData::get_rid() const {

	return probe;
}

void GIProbeData::_validate_property(PropertyInfo &property) const {
	if (property.name == "anisotropy_strength") {
		bool anisotropy_enabled = ProjectSettings::get_singleton()->get("rendering/quality/gi_probes/anisotropic");
		if (!anisotropy_enabled) {
			property.usage = PROPERTY_USAGE_NOEDITOR;
		}
	}
}

void GIProbeData::_bind_methods() {

	ClassDB::bind_method(D_METHOD("allocate", "to_cell_xform", "aabb", "octree_size", "octree_cells", "octree_data", "octree_level_count"), &GIProbeData::allocate);

	ClassDB::bind_method(D_METHOD("get_bounds"), &GIProbeData::get_bounds);
	ClassDB::bind_method(D_METHOD("get_octree_size"), &GIProbeData::get_octree_size);
	ClassDB::bind_method(D_METHOD("get_to_cell_xform"), &GIProbeData::get_to_cell_xform);
	ClassDB::bind_method(D_METHOD("get_octree_cells"), &GIProbeData::get_octree_cells);
	ClassDB::bind_method(D_METHOD("get_data_cells"), &GIProbeData::get_data_cells);
	ClassDB::bind_method(D_METHOD("get_level_counts"), &GIProbeData::get_level_counts);

	ClassDB::bind_method(D_METHOD("set_dynamic_range", "dynamic_range"), &GIProbeData::set_dynamic_range);
	ClassDB::bind_method(D_METHOD("get_dynamic_range"), &GIProbeData::get_dynamic_range);

	ClassDB::bind_method(D_METHOD("set_energy", "energy"), &GIProbeData::set_energy);
	ClassDB::bind_method(D_METHOD("get_energy"), &GIProbeData::get_energy);

	ClassDB::bind_method(D_METHOD("set_bias", "bias"), &GIProbeData::set_bias);
	ClassDB::bind_method(D_METHOD("get_bias"), &GIProbeData::get_bias);

	ClassDB::bind_method(D_METHOD("set_normal_bias", "bias"), &GIProbeData::set_normal_bias);
	ClassDB::bind_method(D_METHOD("get_normal_bias"), &GIProbeData::get_normal_bias);

	ClassDB::bind_method(D_METHOD("set_propagation", "propagation"), &GIProbeData::set_propagation);
	ClassDB::bind_method(D_METHOD("get_propagation"), &GIProbeData::get_propagation);

	ClassDB::bind_method(D_METHOD("set_anisotropy_strength", "strength"), &GIProbeData::set_anisotropy_strength);
	ClassDB::bind_method(D_METHOD("get_anisotropy_strength"), &GIProbeData::get_anisotropy_strength);

	ClassDB::bind_method(D_METHOD("set_interior", "interior"), &GIProbeData::set_interior);
	ClassDB::bind_method(D_METHOD("is_interior"), &GIProbeData::is_interior);

	ClassDB::bind_method(D_METHOD("set_use_two_bounces", "enable"), &GIProbeData::set_use_two_bounces);
	ClassDB::bind_method(D_METHOD("is_using_two_bounces"), &GIProbeData::is_using_two_bounces);

	ClassDB::bind_method(D_METHOD("_set_data", "data"), &GIProbeData::_set_data);
	ClassDB::bind_method(D_METHOD("_get_data"), &GIProbeData::_get_data);

	ADD_PROPERTY(PropertyInfo(Variant::DICTIONARY, "_data", PROPERTY_HINT_NONE, "", PROPERTY_USAGE_NOEDITOR), "_set_data", "_get_data");

	ADD_PROPERTY(PropertyInfo(Variant::INT, "dynamic_range", PROPERTY_HINT_RANGE, "0,8,0.01"), "set_dynamic_range", "get_dynamic_range");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "energy", PROPERTY_HINT_RANGE, "0,64,0.01"), "set_energy", "get_energy");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "bias", PROPERTY_HINT_RANGE, "0,8,0.01"), "set_bias", "get_bias");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "normal_bias", PROPERTY_HINT_RANGE, "0,8,0.01"), "set_normal_bias", "get_normal_bias");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "propagation", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_propagation", "get_propagation");
	ADD_PROPERTY(PropertyInfo(Variant::REAL, "anisotropy_strength", PROPERTY_HINT_RANGE, "0,1,0.01"), "set_anisotropy_strength", "get_anisotropy_strength");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "use_two_bounces"), "set_use_two_bounces", "is_using_two_bounces");
	ADD_PROPERTY(PropertyInfo(Variant::BOOL, "interior"), "set_interior", "is_interior");
}

GIProbeData::GIProbeData() {

	dynamic_range = 4;
	energy = 1.0;
	bias = 1.5;
	normal_bias = 0.0;
	propagation = 0.7;
	anisotropy_strength = 0.5;
	interior = false;

	probe = VS::get_singleton()->gi_probe_create();
}

GIProbeData::~GIProbeData() {

	VS::get_singleton()->free(probe);
}

//////////////////////
//////////////////////

void GIProbe::set_probe_data(const Ref<GIProbeData> &p_data) {

	if (p_data.is_valid()) {
		VS::get_singleton()->instance_set_base(get_instance(), p_data->get_rid());
	} else {
		VS::get_singleton()->instance_set_base(get_instance(), RID());
	}

	probe_data = p_data;
}

Ref<GIProbeData> GIProbe::get_probe_data() const {

	return probe_data;
}

void GIProbe::set_subdiv(Subdiv p_subdiv) {

	ERR_FAIL_INDEX(p_subdiv, SUBDIV_MAX);
	subdiv = p_subdiv;
	update_gizmo();
}

GIProbe::Subdiv GIProbe::get_subdiv() const {

	return subdiv;
}

void GIProbe::set_extents(const Vector3 &p_extents) {

	extents = p_extents;
	update_gizmo();
	_change_notify("extents");
}

Vector3 GIProbe::get_extents() const {

	return extents;
}

void GIProbe::_find_meshes(Node *p_at_node, List<PlotMesh> &plot_meshes) {

	MeshInstance *mi = Object::cast_to<MeshInstance>(p_at_node);
	if (mi && mi->get_flag(GeometryInstance::FLAG_USE_BAKED_LIGHT) && mi->is_visible_in_tree()) {
		Ref<Mesh> mesh = mi->get_mesh();
		if (mesh.is_valid()) {

			AABB aabb = mesh->get_aabb();

			Transform xf = get_global_transform().affine_inverse() * mi->get_global_transform();

			if (AABB(-extents, extents * 2).intersects(xf.xform(aabb))) {
				PlotMesh pm;
				pm.local_xform = xf;
				pm.mesh = mesh;
				for (int i = 0; i < mesh->get_surface_count(); i++) {
					pm.instance_materials.push_back(mi->get_surface_material(i));
				}
				pm.override_material = mi->get_material_override();
				plot_meshes.push_back(pm);
			}
		}
	}

	Spatial *s = Object::cast_to<Spatial>(p_at_node);
	if (s) {

		if (s->is_visible_in_tree()) {

			Array meshes = p_at_node->call("get_meshes");
			for (int i = 0; i < meshes.size(); i += 2) {

				Transform mxf = meshes[i];
				Ref<Mesh> mesh = meshes[i + 1];
				if (!mesh.is_valid())
					continue;

				AABB aabb = mesh->get_aabb();

				Transform xf = get_global_transform().affine_inverse() * (s->get_global_transform() * mxf);

				if (AABB(-extents, extents * 2).intersects(xf.xform(aabb))) {
					PlotMesh pm;
					pm.local_xform = xf;
					pm.mesh = mesh;
					plot_meshes.push_back(pm);
				}
			}
		}
	}

	for (int i = 0; i < p_at_node->get_child_count(); i++) {
		Node *child = p_at_node->get_child(i);
		_find_meshes(child, plot_meshes);
	}
}

GIProbe::BakeBeginFunc GIProbe::bake_begin_function = NULL;
GIProbe::BakeStepFunc GIProbe::bake_step_function = NULL;
GIProbe::BakeEndFunc GIProbe::bake_end_function = NULL;

void GIProbe::bake(Node *p_from_node, bool p_create_visual_debug) {

	static const int subdiv_value[SUBDIV_MAX] = { 6, 7, 8, 9 };

	Voxelizer baker;

	baker.begin_bake(subdiv_value[subdiv], AABB(-extents, extents * 2.0));

	List<PlotMesh> mesh_list;

	_find_meshes(p_from_node ? p_from_node : get_parent(), mesh_list);

	if (bake_begin_function) {
		bake_begin_function(mesh_list.size() + 1);
	}

	int pmc = 0;

	for (List<PlotMesh>::Element *E = mesh_list.front(); E; E = E->next()) {

		if (bake_step_function) {
			bake_step_function(pmc, RTR("Plotting Meshes") + " " + itos(pmc) + "/" + itos(mesh_list.size()));
		}

		pmc++;

		baker.plot_mesh(E->get().local_xform, E->get().mesh, E->get().instance_materials, E->get().override_material);
	}
	if (bake_step_function) {
		bake_step_function(pmc++, RTR("Finishing Plot"));
	}

	baker.end_bake();

	//create the data for visual server

	if (p_create_visual_debug) {
		MultiMeshInstance *mmi = memnew(MultiMeshInstance);
		mmi->set_multimesh(baker.create_debug_multimesh());
		add_child(mmi);
#ifdef TOOLS_ENABLED
		if (get_tree()->get_edited_scene_root() == this) {
			mmi->set_owner(this);
		} else {
			mmi->set_owner(get_owner());
		}
#else
		mmi->set_owner(get_owner());
#endif

	} else {

		Ref<GIProbeData> probe_data = get_probe_data();

		if (probe_data.is_null())
			probe_data.instance();

		probe_data->allocate(baker.get_to_cell_space_xform(), AABB(-extents, extents * 2.0), baker.get_giprobe_octree_size(), baker.get_giprobe_octree_cells(), baker.get_giprobe_data_cells(), baker.get_giprobe_level_cell_count());

		set_probe_data(probe_data);
		probe_data->set_edited(true); //so it gets saved
	}

	if (bake_end_function) {
		bake_end_function();
	}
}

void GIProbe::_debug_bake() {

	bake(NULL, true);
}

AABB GIProbe::get_aabb() const {

	return AABB(-extents, extents * 2);
}

PoolVector<Face3> GIProbe::get_faces(uint32_t p_usage_flags) const {

	return PoolVector<Face3>();
}

String GIProbe::get_configuration_warning() const {

	if (OS::get_singleton()->get_current_video_driver() == OS::VIDEO_DRIVER_GLES2) {
		return TTR("GIProbes are not supported by the GLES2 video driver.\nUse a BakedLightmap instead.");
	}
	return String();
}

void GIProbe::_bind_methods() {

	ClassDB::bind_method(D_METHOD("set_probe_data", "data"), &GIProbe::set_probe_data);
	ClassDB::bind_method(D_METHOD("get_probe_data"), &GIProbe::get_probe_data);

	ClassDB::bind_method(D_METHOD("set_subdiv", "subdiv"), &GIProbe::set_subdiv);
	ClassDB::bind_method(D_METHOD("get_subdiv"), &GIProbe::get_subdiv);

	ClassDB::bind_method(D_METHOD("set_extents", "extents"), &GIProbe::set_extents);
	ClassDB::bind_method(D_METHOD("get_extents"), &GIProbe::get_extents);

	ClassDB::bind_method(D_METHOD("bake", "from_node", "create_visual_debug"), &GIProbe::bake, DEFVAL(Variant()), DEFVAL(false));
	ClassDB::bind_method(D_METHOD("debug_bake"), &GIProbe::_debug_bake);
	ClassDB::set_method_flags(get_class_static(), _scs_create("debug_bake"), METHOD_FLAGS_DEFAULT | METHOD_FLAG_EDITOR);

	ADD_PROPERTY(PropertyInfo(Variant::INT, "subdiv", PROPERTY_HINT_ENUM, "64,128,256,512"), "set_subdiv", "get_subdiv");
	ADD_PROPERTY(PropertyInfo(Variant::VECTOR3, "extents"), "set_extents", "get_extents");
	ADD_PROPERTY(PropertyInfo(Variant::OBJECT, "data", PROPERTY_HINT_RESOURCE_TYPE, "GIProbeData", PROPERTY_USAGE_DEFAULT | PROPERTY_USAGE_DO_NOT_SHARE_ON_DUPLICATE), "set_probe_data", "get_probe_data");

	BIND_ENUM_CONSTANT(SUBDIV_64);
	BIND_ENUM_CONSTANT(SUBDIV_128);
	BIND_ENUM_CONSTANT(SUBDIV_256);
	BIND_ENUM_CONSTANT(SUBDIV_512);
	BIND_ENUM_CONSTANT(SUBDIV_MAX);
}

GIProbe::GIProbe() {

	subdiv = SUBDIV_128;
	extents = Vector3(10, 10, 10);

	gi_probe = VS::get_singleton()->gi_probe_create();
	set_disable_scale(true);
}

GIProbe::~GIProbe() {
	VS::get_singleton()->free(gi_probe);
}
