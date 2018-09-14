#include "osbinding.h"

OSClient::OSClient(string host_url, string username, string password) : m_host_url{ host_url }, rest_prefix{ "/rest" }, m_username{ username }, m_password{ password } {
	Connect();
}

OSClient::~OSClient() {
	Disconnect();
}

void OSClient::Connect() {
	CURL *curl;
	CURLcode res;
	string full_url = m_host_url + rest_prefix + URI_SESSION;
	json data = { { "userName" , m_username },{ "password" , m_password } };

	curl = curl_easy_init();
	if (curl) {
		string json_data = data.dump();

		//setting custom headers
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, a_header);
		headers = curl_slist_append(headers, ct_header);
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		//setting url and data
		
		curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data.c_str());

		//buffer to write json to
		struct buffer b;
		init_buffer(&b);

		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writefunc);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, &b);

		//perform request
		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}

		//get error code
		long http_code = 0;
		curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);

		string output(b.ptr);
		json j = json::parse(output);

		if (http_code != 200) {
			throw j["message"].get<string>();
		}
		else {
			auth_header += j["token"].get<string>();
			user_uri = j["userUri"].get<string>();
		}
		//cleanup
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
	}

}

void OSClient::Disconnect() {
	CURL *curl;
	CURLcode res;
	string full_url = m_host_url + rest_prefix + URI_SESSION;

	curl = curl_easy_init();
	if (curl) {
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, a_header);
		headers = curl_slist_append(headers, ct_header);
		headers = curl_slist_append(headers, auth_header.c_str());
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
		curl_easy_setopt(curl, CURLOPT_URL, full_url.c_str());
		
		res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}

		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
	}
}

// --------------------------
//
//       Account APIs
//
// --------------------------

json OSClient::GetAccount(string view) {
	return notimplemented(__FUNCTION__);
	/*
	string full_url = m_host_url + rest_prefix + URI_ACCOUNT + "?view=" + view;
	return get(full_url);
	*/
}

// --------------------------
//
//      Appliances APIs
//
// --------------------------

json OSClient::GetAppliances(string name, string region_uri) {
	bool firstParam = true;
	string full_url = m_host_url + rest_prefix + URI_APPLIANCES;
	full_url += makeParam(firstParam, name, NAME);
	full_url += makeParam(firstParam, region_uri, REGION_URI);
	return get(full_url);
}

json OSClient::CreateAppliance(string ep_address, string ep_username, string ep_password, string name, string region_uri, string appliance_type) {
	stringNotEmpty(ep_address, ep_username, ep_password, name, region_uri, appliance_type);
	string full_url = m_host_url + rest_prefix + URI_APPLIANCES;
	json data = {
		{ "endpoint" , {
			{ "address", ep_address },
			{ "password", ep_password},
			{ "username", ep_username} 
		}},
		{ "name" , name },
		{ "regionUri", region_uri },
		{ "type", appliance_type }
	};
	return post(full_url, data);
}

json OSClient::GetAppliance(string appliance_id) {
	stringNotEmpty(appliance_id);
	string full_url = m_host_url + rest_prefix + URI_APPLIANCES + "/" + appliance_id;
	return get(full_url);
}

json OSClient::DeleteAppliance(string appliance_id) {
	stringNotEmpty(appliance_id);
	string full_url = m_host_url + appliance_id;
	return deleteRequest(full_url);
}

json OSClient::UpdateAppliance(string appliance_id, json data) {
	stringNotEmpty(appliance_id);
	string full_url = m_host_url + rest_prefix + URI_APPLIANCES + "/" + appliance_id;
	return patch(full_url, data);
}

// --------------------------
//
//    Billing Accounts APIs
//
// --------------------------

//Query properties: providerTypeUri, name
json OSClient::GetBillingAccounts(string query, string view) {
	string full_url = m_host_url + rest_prefix + URI_BILLING_ACCOUNTS + "?query=" + query + "&view=" + view;
	return full_url;
}

json OSClient::CreateBillingAccount(string api_access_key, string description, string directory_uri, string enrollment_number, string name, string provider_type_uri) {
	stringNotEmpty(api_access_key, description, directory_uri, enrollment_number, name, provider_type_uri);
	string full_url = m_host_url + rest_prefix + URI_BILLING_ACCOUNTS;
	json data = {
		{"apiAccessKey", api_access_key},
		{"description", description},
		{"directoryUri", directory_uri},
		{"enrollmentNumber", enrollment_number},
		{"name", name},
		{"providerTypeUri", provider_type_uri}
	};
	return post(full_url, data);
}

json OSClient::DeleteBillingAccount(string billing_account_id) {
	stringNotEmpty(billing_account_id);
	string full_url = m_host_url + rest_prefix + URI_BILLING_ACCOUNTS + "/" + billing_account_id;
	return deleteRequest(billing_account_id);
}

json OSClient::GetBillingAccount(string billing_account_id, string view) {
	string full_url = m_host_url + rest_prefix + URI_BILLING_ACCOUNTS + "/" + billing_account_id + "?view=" + view;
	return get(full_url);
}

json OSClient::UpdateBillingAccount(string billing_account_id, string op, string path, json value) {
	stringNotEmpty(billing_account_id, op, path);
	string full_url = m_host_url + rest_prefix + URI_CATALOGS + "/" + billing_account_id;
	json data = {
		{ "op", op },
		{ "path", path },
		{ "value", value }
	};
	return patch(full_url, data);
}


// --------------------------
//
//     Catalog Types APIs
//
// --------------------------

json OSClient::GetCatalogTypes() {
	string full_url = m_host_url + rest_prefix + URI_CATALOG_TYPES;
	return get(full_url);
}

// --------------------------
//
//       Catalogs APIs
//
// --------------------------

json OSClient::GetCatalogs(string user_query, string view) {
	bool firstParam = true;
	string full_url = m_host_url + rest_prefix + URI_CATALOGS;
	full_url += makeParam(firstParam, user_query, USER_QUERY);
	full_url += makeParam(firstParam, view, VIEW);
	return get(full_url);
}

json OSClient::CreateCatalog(string access_key, string catalog_type_uri, string name, string password, string secret_key, string url, string username, string zone_uri) {
	stringNotEmpty(access_key, catalog_type_uri, name, password, secret_key, url, username);
	string full_url = m_host_url + rest_prefix + URI_APPLIANCES;
	json data = {
		{"accessKey", access_key},
		{"catalogTypeUri", catalog_type_uri},
		{"name", name},
		{"password", password},
		{"secretKey", secret_key},
		{"url", url},
		{"username", username},
		{"zoneUri", zone_uri}
	};
	return post(full_url, data);
}

json OSClient::GetCatalog(string catalog_id, string view) {
	stringNotEmpty(catalog_id);
	string full_url = m_host_url + rest_prefix + URI_CATALOGS + "/" + catalog_id + "?view=" + view;
	return get(full_url);
}

json OSClient::DeleteCatalog(string catalog_id) {
	return notimplemented(__FUNCTION__);
	/*
	string full_url = m_host_url + rest_prefix + URI_CATALOGS + "/" + catalog_id;
	return deleteRequest(full_url);
	*/
}

json OSClient::UpdateCatalog(string catalog_id, string op, string path, json value) {
	stringNotEmpty(catalog_id, op, path);
	string full_url = m_host_url + rest_prefix + URI_CATALOGS + "/" + catalog_id;
	json data = {
		{"op", op},
		{"path", path},
		{"value", value}
	};
	return post(full_url, data);
}

// --------------------------
//
//      Connect App APIs
//
// --------------------------

json OSClient::GetConnectApp(string os) {
	stringNotEmpty(os);
	string full_url = m_host_url + rest_prefix + URI_CONNECT_APP + "?os=" + os;
	return get(full_url);
}

// --------------------------
//
//      Deployments APIs
//
// --------------------------

//Query properties: zoneUri, projectUri
json OSClient::GetDeployments(string query, string user_query, string view) {
	bool firstParam = true;
	string full_url = m_host_url + rest_prefix + URI_DEPLOYMENTS;
	full_url += makeParam(firstParam, query, QUERY);
	full_url += makeParam(firstParam, user_query, USER_QUERY);
	full_url += makeParam(firstParam, view, VIEW);
	return get(full_url);
}

json OSClient::CreateDeployment(json data) {
	string full_url = m_host_url + rest_prefix + URI_DEPLOYMENTS;
	return post(full_url, data);
}

json OSClient::GetDeployment(string deployment_id, string view) {
	stringNotEmpty(deployment_id);
	string full_url = m_host_url + rest_prefix + URI_DEPLOYMENTS + "/" + deployment_id + "?view=" + view;
	return get(full_url);
}

json OSClient::UpdateDeployment(string deployment_id, string op, string path, json value) {
	stringNotEmpty(deployment_id, op, path);
	string full_url = m_host_url + rest_prefix + URI_DEPLOYMENTS + "/" + deployment_id;
	json data = {
		{ "op", op },
		{ "path", path },
		{ "value", value }
	};
	return post(full_url, data);
}

json OSClient::DeleteDeployment(string deployment_id) {
	stringNotEmpty(deployment_id);
	string full_url = m_host_url + rest_prefix + URI_DEPLOYMENTS + "/" + deployment_id;
	return deleteRequest(full_url);
}

json OSClient::ActionOnDeployment(string deployment_id, action_type type, bool force) {
	stringNotEmpty(deployment_id);
	string full_url = m_host_url + rest_prefix + URI_DEPLOYMENTS + "/" + deployment_id + "/actions";
	json data = {
		{ "force", force },
		{ "type", enumToString(type) }
	};
	return post(full_url, data);
}

json OSClient::GetDeploymentConsole(string deployment_id) {
	stringNotEmpty(deployment_id);
	string full_url = m_host_url + rest_prefix + URI_DEPLOYMENTS + "/" + deployment_id + "/console";
	return get(full_url);
}

json OSClient::GetDeploymentKubeconfig(string deployment_id) {
	stringNotEmpty(deployment_id);
	string full_url = m_host_url + rest_prefix + URI_DEPLOYMENTS + "/" + deployment_id + "/kubeconfig";
	return get(full_url);
}

// --------------------------
//
//        Events APIs
//
// --------------------------

json OSClient::GetEvents(string resource_uri) {
	return notimplemented(__FUNCTION__);
	/*
	string full_url = m_host_url + rest_prefix + URI_EVENTS + "?resourceUri=" + resource_uri;
	get(full_url);
	*/
}

// --------------------------
//
//       Keypairs APIs
//
// --------------------------

json OSClient::GetKeyPair(string region_uri, string project_uri) {
	stringNotEmpty(region_uri, project_uri);
	string full_url = m_host_url + rest_prefix + URI_KEYPAIRS + "?regionUri=" + region_uri + "&projectUri=" + project_uri;
	return get(full_url);
}

// --------------------------
//
//    Membership Roles APIs
//
// --------------------------

json OSClient::GetMembershipRoles() {
	string full_url = m_host_url + rest_prefix + URI_MEMBERSHIP_ROLES;
	return get(full_url);
}

// --------------------------
//
//      Memberships APIs
//
// --------------------------

json OSClient::GetMemberships(string query) {
	stringNotEmpty(query);
	string full_url = m_host_url + rest_prefix + URI_MEMBERSHIPS + "?query=" + query;
	return get(full_url);
}

json OSClient::CreateMembership(string group_uri, string membership_role_uri, string project_uri, string user_uri) {
	stringNotEmpty(group_uri, membership_role_uri, project_uri, user_uri);
	string full_url = m_host_url + rest_prefix + URI_MEMBERSHIPS;
	json data = {
		{ "groupUri" , group_uri },
		{ "membershipRoleUri", membership_role_uri },
		{ "projectUri", project_uri },
		{ "userUri" , user_uri },
	};
	return post(full_url, data);
}

json OSClient::DeleteMembership(string membership_id) {
	return notimplemented(__FUNCTION__);
	/*
	//Bad documentation, unknown parameters, likely the following:
	stringNotEmpty(membership_id);
	string full_url = m_host_url + rest_prefix + URI_MEMBERSHIPS + "/" + membership_id;
	return deleteRequest(full_url);
	*/
}

// --------------------------
//
//        Metrics APIs
//
// --------------------------

json OSClient::GetMetrics(string resource_uri, string category, string group_by, string query, string name,
	string period_start, string period, string period_count, string view, string start, string count) {
	bool firstParam = true;
	string full_url = m_host_url + rest_prefix + URI_METRICS;
	full_url += makeParam(firstParam, resource_uri, RESOURCE_URI);
	full_url += makeParam(firstParam, category, CATEGORY);
	full_url += makeParam(firstParam, group_by, GROUP_BY);
	full_url += makeParam(firstParam, query, QUERY);
	full_url += makeParam(firstParam, name, NAME);
	full_url += makeParam(firstParam, period_start, PERIOD_START);
	full_url += makeParam(firstParam, period, PERIOD);
	full_url += makeParam(firstParam, period_count, PERIOD_COUNT);
	full_url += makeParam(firstParam, view, VIEW);
	full_url += makeParam(firstParam, start, START);
	full_url += makeParam(firstParam, count, COUNT);
	return get(full_url);
	
}

// --------------------------
//
//       Networks APIs
//
// --------------------------

//Query properties: zoneUri, projectUri
json OSClient::GetNetworks(string query) {
	bool firstParam = true;
	string full_url = m_host_url + rest_prefix + URI_NETWORKS;
	full_url += makeParam(firstParam, query, QUERY);
	return get(full_url);
}

json OSClient::GetNetwork(string network_id) {
	stringNotEmpty(network_id);
	string full_url = m_host_url + rest_prefix + URI_NETWORKS + "/" + network_id;
	return get(full_url);
}

json OSClient::UpdateNetwork(string network_id, string op, string path, json value) {
	stringNotEmpty(network_id, op, path);
	string full_url = m_host_url + rest_prefix + URI_NETWORKS + "/" + network_id;
	json data = {
		{ "op", op },
		{ "path", path },
		{ "value", value }
	};
	return post(full_url, data);
}

// --------------------------
//
//    Password Reset APIs
//
// --------------------------

json OSClient::ResetSingleUsePassword(string email) {
	stringNotEmpty(email);
	string full_url = m_host_url + rest_prefix + URI_PASSWORD_RESET;
	json data = { { "email", email } };
	return post(full_url, data);
}

json OSClient::ChangePassword(string password, string token) {
	stringNotEmpty(password, token);
	string full_url = m_host_url + rest_prefix + URI_PASSWORD_RESET;
	json data = { 
		{ "password", password },
		{ "token", token }
	};
	return post(full_url, data);
}

// --------------------------
//
//       Projects APIs
//
// --------------------------

json OSClient::GetProjects(string user_query, string view) {
	string full_url = m_host_url + rest_prefix + URI_PROJECTS + "?userQuery=" + user_query + "&view=" + view;
	return get(full_url);
}

json OSClient::CreateProject(string name, string description, json tag_uris) {
	stringNotEmpty(name, description);
	string full_url = m_host_url + rest_prefix + URI_PROJECTS;
	json data = {
		{ "name" , name },
		{ "description", description },
		{ "tagUris", tag_uris }
	};
	return post(full_url, data);
}

json OSClient::GetProject(string project_id, string view) {
	stringNotEmpty(project_id);
	string full_url = m_host_url + rest_prefix + URI_PROJECTS + "/" + project_id + "?view=" + view;
	return get(full_url);
}

json OSClient::DeleteProject(string project_id) {
	stringNotEmpty(project_id);
	string full_url = m_host_url + rest_prefix + URI_PROJECTS + "/" + project_id;
	return deleteRequest(full_url);
}

json OSClient::UpdateProject(string project_id, string name, string description, json tag_uris) {
	stringNotEmpty(project_id, name, description);
	string full_url = m_host_url + rest_prefix + URI_PROJECTS + "/" + project_id;
	json data = {
		{ "name" , name },
		{ "description", description },
		{ "tagUris", tag_uris }
	};
	return patch(full_url, data);
}

// --------------------------
//
//    Provider Types APIs
//
// --------------------------

json OSClient::GetProviderTypes() {
	string full_url = m_host_url + rest_prefix + URI_PROVIDER_TYPES;
	return get(full_url);
}

// --------------------------
//
//       Providers APIs
//
// --------------------------

//Query properties: providerTypeUri, projectUri
json OSClient::GetProviders(string query) {
	bool firstParam = true;
	string full_url = m_host_url + rest_prefix + URI_PROVIDERS + makeParam(firstParam, query, QUERY);
	return get(full_url);
}

json OSClient::CreateProvider(json data) {
	string full_url = m_host_url + rest_prefix + URI_PROVIDERS;
	return post(full_url, data);
}

json OSClient::GetProvider(string provider_id, string view, bool discover) {
	stringNotEmpty(provider_id);
	bool firstParam = true;
	string full_url = m_host_url + rest_prefix + URI_PROVIDERS + "/" + provider_id;
	full_url += makeParam(firstParam, view, VIEW);
	full_url += makeParam(firstParam, boolToString(discover), DISCOVER);
	return get(full_url);
}

json OSClient::DeleteProvider(string provider_id) {
	stringNotEmpty(provider_id);
	string full_url = m_host_url + rest_prefix + URI_PROVIDERS + "/" + provider_id;
	return deleteRequest(full_url);
}

json OSClient::UpdateProvider(string provider_id, string op, string path, json value) {
	stringNotEmpty(provider_id, op, path);
	string full_url = m_host_url + rest_prefix + URI_PROVIDERS + "/" + provider_id;
	json data = {
		{ "op", op },
		{ "path", path },
		{ "value", value }
	};
	return patch(full_url, data);
}

// --------------------------
//
//        Rates APIs
//
// --------------------------

json OSClient::GetRates(string resource_uri, string effective_for_date, string effective_date, string metric_name, bool active, int start, int count) {
	bool firstParam = true;
	string full_url = m_host_url + rest_prefix + URI_RATES;
	full_url += makeParam(firstParam, resource_uri, RESOURCE_URI);
	full_url += makeParam(firstParam, effective_for_date, EFFECTIVE_FOR_DATE);
	full_url += makeParam(firstParam, effective_date, EFFECTIVE_DATE);
	full_url += makeParam(firstParam, boolToString(active), ACTIVE);
	if (start > 0) {
		full_url = full_url + "&start=" + (char)start;
	}
	if (count > 0) {
		full_url = full_url + "&count=" + (char)count;
	}
	return get(full_url);
}

json OSClient::GetRate(string rate_id) {
	stringNotEmpty(rate_id);
	string full_url = m_host_url + rest_prefix + URI_RATES + "/" + rate_id;
	return get(full_url);
}

// --------------------------
//
//       Regions APIs
//
// --------------------------

//Query properties: providerUri
json OSClient::GetRegions(string query, string view) {
	string full_url = m_host_url + rest_prefix + URI_REGIONS + "?query=" + query + "&view=" + view;
	return get(full_url);
}

json OSClient::CreateRegion(string name, string provider_uri, double loc_latitude, double loc_longitude) {
	stringNotEmpty(name, provider_uri);
	string full_url = m_host_url + rest_prefix + URI_REGIONS;
	json data = {
		{ "name" , name },
		{ "providerUri", provider_uri },
		{ "location", {
			{ "latitude" , loc_latitude },
			{ "longitude" , loc_longitude }
		}},
	};
	return post(full_url, data);
}

json OSClient::GetRegion(string region_id, string view, bool discover) {
	stringNotEmpty(region_id);
	bool firstParam = true;
	string full_url = m_host_url + rest_prefix + URI_REGIONS + "/";
	full_url += makeParam(firstParam, view, VIEW);
	full_url += makeParam(firstParam, boolToString(discover), DISCOVER);
	return get(full_url);
}

json OSClient::DeleteRegion(string region_id, bool force) {
	stringNotEmpty(region_id);
	string full_url = m_host_url + rest_prefix + URI_REGIONS + "/" + region_id + "?force=" + boolToString(force);
	return deleteRequest(full_url);
}

json OSClient::UpdateRegion(string region_id, string op, string path, json value) {
	stringNotEmpty(region_id, op, path);
	string full_url = m_host_url + rest_prefix + URI_REGIONS + "/" + region_id;
	json data = {
		{ "op", op },
		{ "path", path },
		{ "value", value }
	};
	return patch(full_url, data);
}

json OSClient::GetRegionConnection(string region_id) {
	stringNotEmpty(region_id);
	string full_url = m_host_url + rest_prefix + URI_REGIONS + "/" + region_id + "/connection";
	return get(full_url);
}

json OSClient::CreateRegionConnection(string region_id, string endpoint_uuid, string name, string loc_ipaddress, string loc_username, string loc_password, string loc_port, string state, string uri) {
	stringNotEmpty(region_id, endpoint_uuid, name, loc_ipaddress, loc_username, loc_password, loc_port, state, uri);
	string full_url = m_host_url + rest_prefix + URI_REGIONS + "/" + region_id + "/connection";
	json data = {
		{ "endpointUuid" , endpoint_uuid },
		{ "name" , name },
		{ "location",{
			{ "ipAddress" , loc_ipaddress },
			{ "username" , loc_username },
			{ "password" , loc_password },
			{ "port" , loc_port },
		} },
		{ "state" , state },
		{ "uri" , uri }
	};
	return post(full_url, data);
}

json OSClient::DeleteRegionConnection(string region_id) {
	stringNotEmpty(region_id);
	string full_url = m_host_url + rest_prefix + URI_REGIONS + "/" + region_id + "/connection";
	return deleteRequest(full_url);
}

json OSClient::GetRegionConnectorImage(string region_id) {
	stringNotEmpty(region_id);
	string full_url = m_host_url + rest_prefix + URI_REGIONS + "/" + region_id + "/connector-image";
	return get(full_url);
}

// --------------------------
//
//         Roles APIs
//
// --------------------------

json OSClient::GetRoles() {
	string full_url = m_host_url + rest_prefix + URI_ROLES;
	return get(full_url);
}

// --------------------------
//
//        Servers APIs
//
// --------------------------

json OSClient::GetServers(string region_uri, string appliance_uri, string zone_uri) {
	bool firstParam = true;
	string full_url = m_host_url + rest_prefix + URI_SERVERS;
	full_url += makeParam(firstParam, region_uri, REGION_URI);
	full_url += makeParam(firstParam, appliance_uri, APPLIANCE_URI);
	full_url += makeParam(firstParam, zone_uri, ZONE_URI);
	return get(full_url);
}

json OSClient::CreateServer(json data) {
	string full_url = m_host_url + rest_prefix + URI_SERVERS;
	return post(full_url, data);
}

json OSClient::DeleteServer(string server_id, bool force) {
	stringNotEmpty(server_id);
	string full_url = m_host_url + rest_prefix + URI_SERVERS + "/" + server_id + "?force=" + boolToString(force);;
	return deleteRequest(full_url);
}

json OSClient::GetServer(string server_id) {
	stringNotEmpty(server_id);
	string full_url = m_host_url + rest_prefix + URI_SERVERS + "/" + server_id;
	return get(full_url);
}

json OSClient::UpdateServer(string server_id, string op, string path, json value) {
	stringNotEmpty(server_id, op, path);
	string full_url = m_host_url + rest_prefix + URI_SERVERS + "/" + server_id;
	json data = {
		{ "op", op },
		{ "path", path },
		{ "value", value }
	};
	return patch(full_url, data);
}


// --------------------------
//
//     Service Types APIs
//
// --------------------------

json OSClient::GetServiceTypes() {
	string full_url = m_host_url + rest_prefix + URI_SERVICE_TYPES;
	return get(full_url);
}

json OSClient::GetServiceType(string service_type_id) {
	stringNotEmpty(service_type_id);
	string full_url = m_host_url + rest_prefix + URI_SERVICE_TYPES + "/" + service_type_id;
	return get(full_url);
}

// --------------------------
//
//		 Services APIs
//
// --------------------------

//Query properties: catalogUri, serviceTypeUri
json OSClient::GetServices(string query, string user_query, string view) {
	string full_url = m_host_url + rest_prefix + URI_SERVICES + "?query=" + query + "&userQuery=" + user_query + "&view=" + view;
	return get(full_url);
}

json OSClient::GetService(string service_id, string view) {
	stringNotEmpty(service_id);
	string full_url = m_host_url + rest_prefix + URI_SERVICES + "/" + service_id + "?view=" + view;
	return get(full_url);
}

// --------------------------
//
//		  Session APIs
//
// --------------------------

json OSClient::GetSession(string view) {
	string full_url = m_host_url + rest_prefix + URI_SESSION + "?view=" + view;
	return get(full_url);
}

json OSClient::GetSessionIdp(string user_name) {
	return notimplemented(__FUNCTION__);
	/*
	string full_url = m_host_url + rest_prefix + URI_SESSION + "/idp" + "?userName=" + user_name;
	return get(full_url);
	*/
}

json OSClient::GetSessionSSO() {
	string full_url = m_host_url + rest_prefix + URI_SESSION + "/sso";
	return get(full_url);
}

json OSClient::CallbackSessionSSO(string token) {
	string full_url = m_host_url + rest_prefix + URI_SESSION + "/sso?token=" + token;
	json data;
	return post(full_url, data);
}

// --------------------------
//
//		  Status APIs
//
// --------------------------

json OSClient::GetStatus() {
	string full_url = m_host_url + rest_prefix + URI_STATUS;
	return get(full_url);
}

// --------------------------
//
//		 Tag Keys APIs
//
// --------------------------

json OSClient::GetTagKeys(string view) {
	string full_url = m_host_url + rest_prefix + URI_TAG_KEYS + "?view=" + view;
	return get(full_url);
}

json OSClient::CreateTagKey(string name) {
	stringNotEmpty(name);
	string full_url = m_host_url + rest_prefix + URI_TAG_KEYS;
	json data = { { "name" , name } };
	return post(full_url, data);
}

json OSClient::GetTagKey(string tag_key_id, string view) {
	stringNotEmpty(tag_key_id);
	string full_url = m_host_url + rest_prefix + URI_TAG_KEYS + "/" + tag_key_id + "?view=" + view;
	return get(full_url);
}

json OSClient::DeleteTagKey(string tag_key_id) {
	stringNotEmpty(tag_key_id);
	string full_url = m_host_url + rest_prefix + URI_TAG_KEYS + "/" + tag_key_id;
	return deleteRequest(full_url);
}

// --------------------------
//
//		   Tags APIs
//
// --------------------------

json OSClient::GetTags(string view) {
	string full_url = m_host_url + rest_prefix + URI_TAGS + "?view=" + view;
	return get(full_url);
}

json OSClient::CreateTag(string name, string tag_key_uri) {
	stringNotEmpty(name, tag_key_uri);
	string full_url = m_host_url + rest_prefix + URI_TAGS;
	json data = {
		{ "name" , name },
		{ "tagKeyUri", tag_key_uri }
	};
	return post(full_url, data);
}

json OSClient::GetTag(string tag_id, string view) {
	stringNotEmpty(tag_id);
	string full_url = m_host_url + rest_prefix + URI_TAGS + "/" + tag_id + "?view=" + view;
	return get(full_url);
}

json OSClient::DeleteTag(string tag_id) {
	stringNotEmpty(tag_id);
	string full_url = m_host_url + rest_prefix + URI_TAGS + "/" + tag_id;
	return deleteRequest(full_url);
}

// --------------------------
//
//		   Users APIs
//
// --------------------------

json OSClient::GetUsers(string user_query) {
	string full_url = m_host_url + rest_prefix + URI_USERS + "?userQuery=" + user_query;
	return get(full_url);
}

json OSClient::CreateUser(string email, string name, string password, string role) {
	stringNotEmpty(email, name, password, role);
	string full_url = m_host_url + rest_prefix + URI_USERS;
	json data = {
		{ "email" , email },
		{ "name", name },
		{ "password", password },
		{ "role", role }
	};
	return post(full_url, data);
}
json OSClient::GetUser(string user_id) {
	stringNotEmpty(user_id);
	string full_url = m_host_url + rest_prefix + URI_USERS + "/" + user_id;
	return get(full_url);
}

json OSClient::UpdateUser(string user_id, string email, string name, string password, string role) {
	stringNotEmpty(user_id, email, name, password, role);
	string full_url = m_host_url + rest_prefix + URI_USERS + "/" + user_id;
	json data = {
		{ "email" , email },
		{ "name", name },
		{ "password", password },
		{ "role", role }
	};
	return patch(full_url, data);
}

json OSClient::DeleteUser(string user_id) {
	stringNotEmpty(user_id);
	string full_url = m_host_url + rest_prefix + URI_USERS + "/" + user_id;
	return deleteRequest(full_url);
}

// --------------------------
//
//		  Version APIs
//
// --------------------------

json OSClient::GetVersion() {
	string full_url = m_host_url + rest_prefix + URI_VERSIONS;
	return get(full_url);
}

// --------------------------
//
// Virtual Machine Profiles APIs
//
// --------------------------

json OSClient::GetVirtualMachineProfiles(string zone_uri, string service_uri) {
	string full_url = m_host_url + rest_prefix + URI_VIRTUAL_MACHINE_PROFILES + "?zoneUri=" + zone_uri + "&serviceUri=" + service_uri;
	return get(full_url);
}

json OSClient::GetVirtualMachineProfile(string vm_profile_id) {
	stringNotEmpty(vm_profile_id);
	string full_url = m_host_url + rest_prefix + URI_VIRTUAL_MACHINE_PROFILES + "/" + vm_profile_id;
	return get(full_url);
}

// --------------------------
//
//		  Volumes APIs
//
// --------------------------

json OSClient::GetVolumes(string query, string view) {
	string full_url = m_host_url + rest_prefix + URI_VOLUMES + "?query=" + query + "&view=" + view;
	return get(full_url);
}

json OSClient::CreateVolume(string name, int size_gib, string zone_uri, string project_uri) {
	string full_url = m_host_url + rest_prefix + URI_VOLUMES;
	json data = {
		{ "name", name },
		{ "sizeGiB", size_gib },
		{ "zoneUri", zone_uri },
		{ "projectUri", project_uri }
	};
	return post(full_url, data);
}

json OSClient::GetVolume(string volume_id) {
	stringNotEmpty(volume_id);
	string full_url = m_host_url + rest_prefix + URI_VOLUMES + "/" + volume_id;
	return get(full_url);
}

json OSClient::UpdateVolume(string volume_id, string op, string path, json value) {
	stringNotEmpty(volume_id, op, path);
	string full_url = m_host_url + rest_prefix + URI_VOLUMES + "/" + volume_id;
	json data = {
		{ "op", op },
		{ "path", path },
		{ "value", value }
	};
	return patch(full_url, data);
}

json OSClient::DeleteVolume(string volume_id) {
	stringNotEmpty(volume_id);
	string full_url = m_host_url + rest_prefix + URI_VOLUMES + "/" + volume_id;
	return deleteRequest(full_url);
}

// --------------------------
//
//		 Zone Types APIs
//
// --------------------------

json OSClient::GetZoneTypes() {
	string full_url = m_host_url + rest_prefix + URI_ZONE_TYPES;
	return get(full_url);
}

json OSClient::GetZoneTypeResourceProfiles(string zone_type_id) {
	string full_url = m_host_url + rest_prefix + URI_ZONE_TYPES + "/" + zone_type_id + "/resource-profiles";
	return get(full_url);
}

// --------------------------
//
//		   Zones APIs
//
// --------------------------

json OSClient::GetZones(string query, string region_uri, string provider_uri, string appliance_uri, string view) {
	bool firstParam = true;
	string full_url = m_host_url + rest_prefix + URI_ZONES;
	full_url += makeParam(firstParam, query, QUERY);
	full_url += makeParam(firstParam, region_uri, REGION_URI);
	full_url += makeParam(firstParam, provider_uri, PROVIDER_URI);
	full_url += makeParam(firstParam, appliance_uri, APPLIANCE_URI);
	full_url += makeParam(firstParam, view, VIEW);
	return get(full_url);
}

json OSClient::CreateZone(json zone_data) {
	string full_url = m_host_url + rest_prefix + URI_ZONES;
	return post(full_url, zone_data);
}

json OSClient::GetZone(string zone_id, string view) {
	stringNotEmpty(zone_id);
	string full_url = m_host_url + rest_prefix + URI_ZONES + "/" + zone_id + "?view=" + view;
	return get(full_url);
}

json OSClient::UpdateZone(string zone_id, string op, string path, json value) {
	stringNotEmpty(zone_id, op, path);
	string full_url = m_host_url + rest_prefix + URI_ZONES + "/" + zone_id;
	json data = {
		{ "op", op },
		{ "path", path },
		{ "value", value }
	};
	return patch(full_url, data);
}

json OSClient::DeleteZone(string zone_id, bool force) {
	string full_url = m_host_url + rest_prefix + URI_ZONES + "/" + zone_id;
	return deleteRequest(full_url);
}

json OSClient::ActionOnZone(string zone_id, action_type type, string resource_type, int resource_capacity) {
	stringNotEmpty(zone_id);
	string full_url = m_host_url + rest_prefix + URI_ZONES + "/" + zone_id + "/actions";
	json data = {
		{ "type", enumToString(type) },
		{ "resourceOps", {
			{ "resourceType", resource_type },
			{ "resourceCapacity", resource_capacity }
		}}
	};
	return post(full_url, data);
}

json OSClient::GetZoneApplianceImage(string zone_id) {
	stringNotEmpty(zone_id);
	string full_url = m_host_url + rest_prefix + URI_ZONES + "/" + zone_id + "/appliance-image";
	return get(full_url);
}

json OSClient::GetZoneTaskStatus(string zone_id) {
	stringNotEmpty(zone_id);
	string full_url = m_host_url + rest_prefix + URI_ZONES + "/" + zone_id + "/task-status";
	return get(full_url);
}

json OSClient::GetZoneConnections(string zone_id, string uuid) {
	stringNotEmpty(zone_id, uuid);
	string full_url = m_host_url + rest_prefix + URI_ZONES + "/" + zone_id + "/connections" + "?uuid=" + uuid;
	return get(full_url);
}

json OSClient::CreateZoneConnection(string zone_id, string uuid, string name, string ip_address, string username, string password, string port, string state) {
	stringNotEmpty(zone_id);
	string full_url = m_host_url + rest_prefix + URI_ZONES + "/" + zone_id + "/connections";
	json data = {
		{ "uuid", uuid },
		{ "name", name },
		{ "location", {
			{ "ipAddress", ip_address },
			{ "username", username },
			{ "password", password },
			{ "port", port },
		}},
	{ "state", state },
	};
	return post(full_url, data);
}

json OSClient::DeleteZoneConnection(string zone_id, string uuid) {
	stringNotEmpty(zone_id, uuid);
	string full_url = m_host_url + rest_prefix + URI_ZONES + "/" + zone_id + "/connections/" + uuid;
	return deleteRequest(full_url);
}

json OSClient::UpdateZoneConnection(string zone_id, string uuid, string op, string path, json value) {
	stringNotEmpty(zone_id, uuid, op, path);
	string full_url = m_host_url + rest_prefix + URI_ZONES + "/" + zone_id + "/connections/" + uuid;
	json data = {
		{ "op", op },
		{ "path", path },
		{ "value" , value }
	};
	return patch(full_url, data);
}

json OSClient::GetZoneEndpoint(string zone_id) {
	stringNotEmpty(zone_id);
	string full_url = m_host_url + rest_prefix + URI_ZONES + "/" + zone_id + "/os-endpoints";
	return get(full_url);
}