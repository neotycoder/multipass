/*
 * Copyright (C) 2021 Canonical, Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "base_virtual_machine_factory.h"

#include <multipass/cloud_init_iso.h>
#include <multipass/utils.h>

#include <yaml-cpp/yaml.h>

namespace mp = multipass;
namespace mpu = multipass::utils;

QString mp::BaseVirtualMachineFactory::make_cloud_init_image(const std::string& name, const QDir& instance_dir,
                                                             const YAML::Node& meta_data_config,
                                                             const YAML::Node& user_data_config,
                                                             const YAML::Node& vendor_data_config,
                                                             const YAML::Node& network_data_config)
{
    const auto cloud_init_iso = instance_dir.filePath("cloud-init-config.iso");
    if (QFile::exists(cloud_init_iso))
        return cloud_init_iso;

    mp::CloudInitIso iso;
    iso.add_file("meta-data", mpu::emit_cloud_config(meta_data_config));
    iso.add_file("vendor-data", mpu::emit_cloud_config(vendor_data_config));
    iso.add_file("user-data", mpu::emit_cloud_config(user_data_config));
    if (!network_data_config.IsNull())
        iso.add_file("network-config", mpu::emit_cloud_config(network_data_config));

    iso.write_to(cloud_init_iso);

    return cloud_init_iso;
}
