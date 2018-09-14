#ifndef OSBINDING_H
#define OSBINDING_H

#include "nlohmann/json.hpp"
#include <curl.h>

#include "globals.h"

using namespace std;
using json = nlohmann::json;

class OSClient {
public:
	OSClient(string host_url, string username, string password);
	~OSClient();

	void Connect();
	void Disconnect();

	//Account APIs
	json GetAccount(string view = "full");  //not implimented

	//Appliances APIs
	json GetAppliances(string name = "", string regionUri = "");
	json CreateAppliance(string ep_address, string ep_username, string ep_password, string name, string region_uri, string appliance_type);
	json GetAppliance(string appliance_id);
	json DeleteAppliance(string appliance_id);
	json UpdateAppliance(string appliance_id, json data);

	//Billing Accounts APIs
	json GetBillingAccounts(string query = "", string view = "");
	json CreateBillingAccount(string api_access_key, string description, string directory_uri, string enrollment_number, string name, string provider_type_uri);
	json DeleteBillingAccount(string billing_account_id);
	json GetBillingAccount(string billing_account_id, string view = "");
	json UpdateBillingAccount(string billing_account_id, string op, string path, json value);

	//Catalog Types APIs
	json GetCatalogTypes();

	//Catalogs APIs
	json GetCatalogs(string user_query = "", string view = "");
	json CreateCatalog(string access_key, string catalog_type_uri, string name, string password, string secret_key, string url, string username, string zone_uri);
	json GetCatalog(string catalog_id, string view);
	json DeleteCatalog(string catalog_id);  //not implimented
	json UpdateCatalog(string catalog_id, string op, string path, json value);

	//Connect App APIs
	json GetConnectApp(string os = "windows");

	//Deployments APIs
	json GetDeployments(string query = "", string user_query = "", string view = "");
	json CreateDeployment(json data);
	json GetDeployment(string deployment_id, string view = "full");
	json UpdateDeployment(string deployment_id, string op, string path, json value);
	json DeleteDeployment(string deployment_id);
	json ActionOnDeployment(string deployment_id, action_type type, bool force = true);
	json GetDeploymentConsole(string deployment_id);
	json GetDeploymentKubeconfig(string deployment_id);

	//Events APIs
	json GetEvents(string resource_uri);  //not implimented

	//Keypairs APIs
	json GetKeyPair(string region_uri, string project_uri);

	//Membership Roles APIs
	json GetMembershipRoles();

	//Memberships APIs
	json GetMemberships(string query);
	json CreateMembership(string group_uri, string membership_role_uri, string project_uri, string user_uri);
	json DeleteMembership(string membership_id);  //Possibly wrong, bad documentation

	//Metrics APIs
	json GetMetrics(string resource_uri, string category, string group_by, string query, string name,
		string period_start, string period, string period_count, string view, string start, string count);

	//Networks APIs
	json GetNetworks(string query);
	json GetNetwork(string network_id); 
	json UpdateNetwork(string network_id, string op, string path, json value);

	//Password Reset APIs
	json ResetSingleUsePassword(string email);
	json ChangePassword(string password, string token);

	//Projects APIs
	json GetProjects(string user_query = "", string view = "");
	json CreateProject(string name, string description, json tag_uris);
	json GetProject(string project_id, string view);
	json DeleteProject(string project_id);  //not implimented
	json UpdateProject(string project_id, string name, string description, json tag_uris);

	//Provider Types APIs
	json GetProviderTypes();

	//Providers APIs
	json GetProviders(string query = "");
	json CreateProvider(json data);
	json GetProvider(string provider_id, string view, bool discover);
	json DeleteProvider(string provider_id);
	json UpdateProvider(string provider_id, string op, string path, json value);

	//Rates APIs
	json GetRates(string resource_uri = "", string effective_for_date = "", string effective_date = "", string metric_name = "", bool active = true, int start = 0, int count = 0);
	json GetRate(string rate_id);

	//Regions APIs
	json GetRegions(string query = "", string view = "");
	json CreateRegion(string name, string provider_uri, double loc_latitude, double loc_longitude);
	json GetRegion(string region_id, string view, bool discover = true);
	json DeleteRegion(string region_id, bool force = false);
	json UpdateRegion(string region_id, string op, string path, json value);
	json GetRegionConnection(string region_id);
	json CreateRegionConnection(string region_id, string endpoint_uuid, string name, string loc_ipaddress, string loc_username, string loc_password, string loc_port, string state, string uri);
	json DeleteRegionConnection(string region_id);
	json GetRegionConnectorImage(string region_id);

	//Roles APIs
	json GetRoles();

	//Servers APIs
	json GetServers(string region_uri = "", string appliance_uri = "", string zone_uri = "");
	json CreateServer(json data);
	json DeleteServer(string server_id, bool force = false);
	json GetServer(string server_id);
	json UpdateServer(string server_id, string op, string path, json value);

	//Service Types APIs
	json GetServiceTypes();
	json GetServiceType(string service_type_id);

	//Services APIs
	json GetServices(string query = "", string user_query = "", string view = "full");
	json GetService(string service_id, string view = "full");

	//Session APIs
	json GetSession(string view = "full");
	json GetSessionIdp(string user_name);  //not implimented
	json GetSessionSSO();
	json CallbackSessionSSO(string token);  //Post w/query and no body

	//Status APIs
	json GetStatus();

	//Tag Keys APIs
	json GetTagKeys(string view = "full");
	json CreateTagKey(string name);
	json GetTagKey(string tag_key_id, string view = "full");
	json DeleteTagKey(string tag_key_id);

	//Tags APIs
	json GetTags(string view = "full");
	json CreateTag(string name, string tag_key_uri);
	json GetTag(string tag_id, string view = "full");
	json DeleteTag(string tag_id);

	//Users APIs
	json GetUsers(string user_query = "");
	json CreateUser(string email, string name, string password, string role);
	json GetUser(string user_id);
	json UpdateUser(string user_id, string email, string name, string password, string role);
	json DeleteUser(string user_id);

	//Version APIs
	json GetVersion();

	//Virtual Machine Profiles APIs
	json GetVirtualMachineProfiles(string zone_uri = "", string service_uri = "");
	json GetVirtualMachineProfile(string vm_profile_id);

	//Volumes APIs
	json GetVolumes(string query = "", string view = "full");
	json CreateVolume(string name, int size_gib, string zone_uri, string project_uri);
	json GetVolume(string volume_id);
	json UpdateVolume(string volume_id, string op, string path, json value);
	json DeleteVolume(string volume_id);

	//Zone Types APIs
	json GetZoneTypes();
	json GetZoneTypeResourceProfiles(string zone_type_id);

	//Zones APIs
	json GetZones(string query = "", string region_uri = "", string provider_uri = "", string appliance_uri = "", string view = "");
	json CreateZone(json zone_data);
	json GetZone(string zone_id, string view = "full");
	json UpdateZone(string zone_id, string op, string path, json value);
	json DeleteZone(string zone_id, bool force = true);
	json ActionOnZone(string zone_id, action_type type, string resource_type, int resource_capacity);
	json GetZoneApplianceImage(string zone_id);
	json GetZoneTaskStatus(string zone_id);
	json GetZoneConnections(string zone_id, string uuid);
	json CreateZoneConnection(string zone_id, string uuid, string name, string ip_address, string username, string password, string port, string state);
	json DeleteZoneConnection(string zone_id, string uuid);
	json UpdateZoneConnection(string zone_id, string uuid, string op, string path, json value);
	json GetZoneEndpoint(string zone_id);

private:
	json get(string url);
	json post(string url, json data);
	json patch(string url, json data);
	json deleteRequest(string url);

	string URI_ACCOUNT = "/account";
	string URI_APPLIANCES = "/appliances";
	string URI_BILLING_ACCOUNTS = "/billing-accounts";
	string URI_CATALOG_TYPES = "/catalog-types";
	string URI_CATALOGS = "/catalogs";
	string URI_CONNECT_APP = "/connect-app";
	string URI_DEPLOYMENTS = "/deployments";
	string URI_EVENTS = "/events";
	string URI_KEYPAIRS = "/keypairs";
	string URI_MEMBERSHIP_ROLES = "/membership-roles";
	string URI_MEMBERSHIPS = "/memberships";
	string URI_METRICS = "/metrics";
	string URI_NETWORKS = "/networks";
	string URI_PASSWORD_RESET = "/password-reset";
	string URI_PROJECTS = "/projects";
	string URI_PROVIDER_TYPES = "/provider-types";
	string URI_PROVIDERS = "/providers";
	string URI_RATES = "/rates";
	string URI_REGIONS = "/regions";
	string URI_ROLES = "/roles";
	string URI_SERVERS = "/servers";
	string URI_SERVICE_TYPES = "/service-types";
	string URI_SERVICES = "/services";
	string URI_SESSION = "/session";
	string URI_SESSION_IDP = "/session/idp";
	string URI_STATUS = "/status";
	string URI_TAG_KEYS = "/tag-keys";
	string URI_TAGS = "/tags";
	string URI_USERS = "/users";
	string URI_VERSIONS = "/about/versions";
	string URI_VIRTUAL_MACHINE_PROFILES = "/virtual-machine-profiles";
	string URI_VOLUMES = "/volumes";
	string URI_WORKSPACES = "/workspaces";
	string URI_ZONE_TYPES = "/zone-types";
	string URI_ZONES = "/zones";

	string APPLIANCE_URI = "applianceUri";
	string PROVIDER_URI = "providerUri";
	string REGION_URI = "regionUri";
	string RESOURCE_URI = "resourceUri";
	string ZONE_URI = "zoneUri";
	string CATEGORY = "category";
	string GROUP_BY = "groupBy";
	string QUERY = "query";
	string USER_QUERY = "userQuery";
	string NAME = "name";
	string PERIOD_START = "periodStart";
	string PERIOD = "period";
	string PERIOD_COUNT = "periodCount";
	string VIEW = "view";
	string START = "start";
	string COUNT = "count";
	string EFFECTIVE_FOR_DATE = "effectiveForDate";
	string EFFECTIVE_DATE = "effectiveDate";
	string METRIC_NAME = "metricName";
	string ACTIVE = "active";
	string DISCOVER = "discover";

	const char *a_header = "Accept: application/json";
	const char *ct_header = "Content-Type: application/json";
	string auth_header = "Authorization: ";

	string m_host_url;
	string rest_prefix;
	string m_username;
	string m_password;
	string user_uri;
};

//Basic get request for most get functions.
//Query parameters are not supported by libcurl, must be implimented manually.
inline json OSClient::get(string url) {
	CURL *curl;
	CURLcode res;
	json returnjson;

	curl = curl_easy_init();
	if (curl) {
		//setting custom headers
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, a_header);
		headers = curl_slist_append(headers, ct_header);
		headers = curl_slist_append(headers, auth_header.c_str());
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		//setting url
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

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

		string output(b.ptr);
		if (b.len > 0) {
			returnjson = json::parse(output);
		}

		curl_easy_cleanup(curl);
	}

	return returnjson;
}

//Basic post request for most create functions.
inline json OSClient::post(string url, json data) {
	CURL *curl;
	CURLcode res;
	json returnjson;

	curl = curl_easy_init();
	if (curl) {
		string json_data = data.dump();

		//setting custom headers
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, a_header);
		headers = curl_slist_append(headers, ct_header);
		headers = curl_slist_append(headers, auth_header.c_str());
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		//setting url and body of post
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
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

		string output(b.ptr);
		if (b.len > 0) {
			returnjson = json::parse(output);
		}

		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
	}
	return returnjson;
}

//Basic patch request for most update methods.
inline json OSClient::patch(string url, json data) {
	CURL *curl;
	CURLcode res;
	json returnjson;

	curl = curl_easy_init();
	if (curl) {
		string json_data = data.dump();

		//setting custom headers
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, a_header);
		headers = curl_slist_append(headers, ct_header);
		headers = curl_slist_append(headers, auth_header.c_str());
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		//setting url and specifying put
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PATCH");
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
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

		string output(b.ptr);
		if (b.len > 0) {
			returnjson = json::parse(output);
		}

		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
	}
	return returnjson;
}

//Basic delete request with for requests without a body.
inline json OSClient::deleteRequest(string url) {
	CURL *curl;
	CURLcode res;
	json returnjson;

	curl = curl_easy_init();
	if (curl) {
		//setting custom headers
		struct curl_slist *headers = NULL;
		headers = curl_slist_append(headers, a_header);
		headers = curl_slist_append(headers, ct_header);
		headers = curl_slist_append(headers, auth_header.c_str());
		res = curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

		//setting url and specifying deletion
		curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

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
		
		string output(b.ptr);
		if (b.len > 0) {
			returnjson = json::parse(output);
		}
		
		curl_easy_cleanup(curl);
		curl_slist_free_all(headers);
	}
	return returnjson;
}

#endif