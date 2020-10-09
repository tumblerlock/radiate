#!/usr/bin/env bash

set -uvo pipefail

echo -n 'Creating directories: www, data, scripts ...'
mkdir -p www/radiate
mkdir data
mkdir scripts
echo 'done!'

set -e

repo_url='https://raw.githubusercontent.com/tumblerlock/radiate/master'
curl_opts='--silent'

echo -n 'Installing helper scripts...'
curl $curl_opts "$repo_url/install/scripts/compress_www" > scripts/compress_www
curl $curl_opts "$repo_url/install/scripts/serve_www" > scripts/serve_www
curl $curl_opts "$repo_url/install/scripts/upload_www" > scripts/upload_www

chmod u+x scripts/compress_www scripts/serve_www scripts/upload_www
echo 'done'

echo -n 'Installing portal web assets...'
curl $curl_opts "$repo_url/install/www/portal.html" > www/radiate/portal.html
curl $curl_opts "$repo_url/install/www/scan.json" > www/radiate/scan.json
echo 'done'

echo -n 'Ammending .gitignore...'
curl $curl_opts "$repo_url/install/gitignore.template" >> .gitignore
echo 'done'
