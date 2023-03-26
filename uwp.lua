import("lib.detect.find_program")
import("detect.sdks.find_vstudio")

local function listFiles(dir, out, i)
    for _, f in ipairs(os.filedirs(path.join(dir, "**"))) do
        if os.isfile(f) then
            t = f
            if i then
                t = string.sub(f, i)
            end
            table.insert(out, {f, t})
        end
    end
end

local outPath = "build/demo.msix"
local keyPath = "winrt/key.pfx"
local priconfigPath = "build/priconfig.xml"
local fileMapPath = "build/main.map.txt"
local priPath = "build\\resources.pri"

function main(target)
    local files = {
        {priPath, "resources.pri"},
        {target:targetfile(), path.filename(target:targetfile())},
    }
    local target_pdb = path.join(target:targetdir(), path.basename(target:targetfile())..".pdb")
    if os.exists(target_pdb) then
        table.insert(files, {target_pdb, path.filename(target_pdb)})
    end
    for _, pkg in pairs(target:pkgs()) do
        if pkg:has_shared() then
            for _, f in ipairs(pkg:libraryfiles()) do
                if f:endswith(".dll") then
                    table.insert(files, {f, path.filename(f)})
                end
            end
        end
    end
    listFiles("winrt/Assets", files, 7)
    listFiles("resources", files)
    local context = ""
    for _, ff in ipairs(files) do
        context = context..format('\n"%s"\t\t"%s"', ff[1], ff[2])
    end

    io.writefile(fileMapPath, format([[
[ResourceMetadata]
"ResourceDimensions"	"language-en-US"
[Files]
%s
]], context))

    local vs = find_vstudio()["2022"]["vcvarsall"]["x86"]
    local windowsSdkBinPath = path.join(vs["WindowsSdkBinPath"], vs["WindowsSDKVersion"], "x86")
    local makepri = path.join(windowsSdkBinPath, "makepri.exe")
    local makeappx = path.join(windowsSdkBinPath, "makeappx.exe")
    local signtool = path.join(windowsSdkBinPath, "signtool.exe")

    os.execv(makepri, {
        "createconfig",
        "-Overwrite",
        "/cf",
        priconfigPath,
        "/dq",
        "en-US"
    })
    os.execv(makepri, {
        "new",
        "-Overwrite",
        "/pr",
        "winrt",
        "/cf",
        priconfigPath,
        "-OutputFile",
        priPath
    })
    os.execv(makeappx, {
        "pack",
        "/l",
        "/h",
        "SHA256",
        "/f",
        fileMapPath,
        "/m",
        "winrt/AppxManifest.xml",
        "/o",
        "/p",
        outPath
    })
    os.execv(signtool, {
        "sign",
        "/fd",
        "SHA256",
        "/a",
        "/f",
        keyPath,
        outPath
    })
end