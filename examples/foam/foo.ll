; ModuleID = 'dz'
source_filename = "dz"

@SDL_INIT_VIDEO = internal global i32 32
@SDL_WINDOWPOS_UNDEFINED = internal global i32 536805376
@SDL_KEYDOWN = internal global i32 768
@SDL_KEYUP = internal global i32 769
@SDL_MOUSEBUTTONDOWN = internal global i32 1025
@SDLK_a = internal global i32 97
@SDLK_d = internal global i32 100
@SDLK_s = internal global i32 115
@SDLK_w = internal global i32 119
@Normal = internal global i32 0
@MouseOver = internal global i32 1
@Pressed = internal global i32 2
@string = private unnamed_addr constant [5 x i8] c"test\00", align 1
@string.1 = private unnamed_addr constant [10 x i8] c"Click me!\00", align 1
@string.2 = private unnamed_addr constant [10 x i8] c"Click me!\00", align 1
@string.3 = private unnamed_addr constant [10 x i8] c"Click me!\00", align 1
@string.4 = private unnamed_addr constant [10 x i8] c"Click me!\00", align 1

define i32 @main() {
alloc:
  %0 = alloca i64, align 8
  %1 = alloca i64, align 8
  %2 = alloca i64, align 8
  %3 = alloca i1, align 1
  %4 = alloca i1, align 1
  %5 = alloca i1, align 1
  %6 = alloca i64, align 8
  %7 = alloca { i32, i32, i32, i32 }, align 8
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca i32, align 4
  %12 = alloca i64, align 8
  %13 = alloca i32, align 4
  %14 = alloca i32, align 4
  %15 = alloca i32, align 4
  %16 = alloca i32, align 4
  %17 = alloca i32, align 4
  %18 = alloca i64, align 8
  %19 = alloca i64, align 8
  %20 = alloca i32, align 4
  %21 = alloca i32, align 4
  %22 = alloca i32, align 4
  %23 = alloca i32, align 4
  %24 = alloca i32, align 4
  %25 = alloca i32, align 4
  %26 = alloca i32, align 4
  %27 = alloca i32, align 4
  %28 = alloca i32, align 4
  %29 = alloca i32, align 4
  %30 = alloca i32, align 4
  %31 = alloca i32, align 4
  %32 = alloca i32, align 4
  %33 = alloca i32, align 4
  %34 = alloca i64, align 8
  %35 = alloca i64, align 8
  %36 = alloca { i32, i32, i32, i32 }, align 8
  %37 = alloca i32, align 4
  %38 = alloca i32, align 4
  %39 = alloca i32, align 4
  %40 = alloca i32, align 4
  %41 = alloca i64, align 8
  %42 = alloca i32, align 4
  %43 = alloca i32, align 4
  %44 = alloca i32, align 4
  %45 = alloca i32, align 4
  %46 = alloca i32, align 4
  %47 = alloca i64, align 8
  %48 = alloca i64, align 8
  %49 = alloca i1, align 1
  %50 = alloca i1, align 1
  %51 = alloca i64, align 8
  %52 = alloca { i32, i32, i32, i32 }, align 8
  %53 = alloca i32, align 4
  %54 = alloca i32, align 4
  %55 = alloca i32, align 4
  %56 = alloca i32, align 4
  %57 = alloca i64, align 8
  %58 = alloca i32, align 4
  %59 = alloca i32, align 4
  %60 = alloca i32, align 4
  %61 = alloca i32, align 4
  %62 = alloca i32, align 4
  %63 = alloca i64, align 8
  %64 = alloca i64, align 8
  %65 = alloca i1, align 1
  %66 = alloca i1, align 1
  %67 = alloca i1, align 1
  %68 = alloca i64, align 8
  %69 = alloca { i32, i32, i32, i32 }, align 8
  %70 = alloca i32, align 4
  %71 = alloca i32, align 4
  %72 = alloca i32, align 4
  %73 = alloca i32, align 4
  %74 = alloca i64, align 8
  %75 = alloca i32, align 4
  %76 = alloca i32, align 4
  %77 = alloca i32, align 4
  %78 = alloca i32, align 4
  %79 = alloca i32, align 4
  %80 = alloca i64, align 8
  %81 = alloca i64, align 8
  %82 = alloca i32, align 4
  %83 = alloca i32, align 4
  %84 = alloca i32, align 4
  %85 = alloca i32, align 4
  %86 = alloca i32, align 4
  %87 = alloca i32, align 4
  %88 = alloca i32, align 4
  %89 = alloca i32, align 4
  %90 = alloca i32, align 4
  %91 = alloca i32, align 4
  %92 = alloca i32, align 4
  %93 = alloca i32, align 4
  %94 = alloca i32, align 4
  %95 = alloca i32, align 4
  %96 = alloca i64, align 8
  %97 = alloca i64, align 8
  %98 = alloca { i32, i32, i32, i32 }, align 8
  %99 = alloca i32, align 4
  %100 = alloca i32, align 4
  %101 = alloca i32, align 4
  %102 = alloca i32, align 4
  %103 = alloca i64, align 8
  %104 = alloca i32, align 4
  %105 = alloca i32, align 4
  %106 = alloca i32, align 4
  %107 = alloca i32, align 4
  %108 = alloca i32, align 4
  %109 = alloca i64, align 8
  %110 = alloca i64, align 8
  %111 = alloca i32, align 4
  %112 = alloca i32, align 4
  %113 = alloca i32, align 4
  %114 = alloca i32, align 4
  %115 = alloca i32, align 4
  %116 = alloca i32, align 4
  %117 = alloca i32, align 4
  %118 = alloca i32, align 4
  %119 = alloca i32, align 4
  %120 = alloca i32, align 4
  %121 = alloca i32, align 4
  %122 = alloca i32, align 4
  %123 = alloca i32, align 4
  %124 = alloca i32, align 4
  %125 = alloca i32, align 4
  %126 = alloca i32, align 4
  %127 = alloca i32, align 4
  %128 = alloca i32, align 4
  %129 = alloca i32, align 4
  %130 = alloca i32, align 4
  %131 = alloca i32, align 4
  %132 = alloca i32, align 4
  %133 = alloca i32, align 4
  %134 = alloca i32, align 4
  %135 = alloca i32, align 4
  %136 = alloca i32, align 4
  %137 = alloca i32, align 4
  %138 = alloca i32, align 4
  %139 = alloca i32, align 4
  %140 = alloca i32, align 4
  %141 = alloca i32, align 4
  %142 = alloca i32, align 4
  %143 = alloca i32, align 4
  %144 = alloca i32, align 4
  %145 = alloca i32, align 4
  %146 = alloca i32, align 4
  %147 = alloca i32, align 4
  %148 = alloca i32, align 4
  %149 = alloca i32, align 4
  %150 = alloca i32, align 4
  %151 = alloca i32, align 4
  %152 = alloca i32, align 4
  %153 = alloca i64, align 8
  %154 = alloca i64, align 8
  %155 = alloca i1, align 1
  %156 = alloca i1, align 1
  %157 = alloca i1, align 1
  %158 = alloca i64, align 8
  %159 = alloca { i32, i32, i32, i32 }, align 8
  %160 = alloca i32, align 4
  %161 = alloca i32, align 4
  %162 = alloca i32, align 4
  %163 = alloca i32, align 4
  %164 = alloca i64, align 8
  %165 = alloca i32, align 4
  %166 = alloca i32, align 4
  %167 = alloca i32, align 4
  %168 = alloca i32, align 4
  %169 = alloca i32, align 4
  %170 = alloca i64, align 8
  %171 = alloca i64, align 8
  %172 = alloca i32, align 4
  %173 = alloca i32, align 4
  %174 = alloca i32, align 4
  %175 = alloca i32, align 4
  %176 = alloca i32, align 4
  %177 = alloca i32, align 4
  %178 = alloca i32, align 4
  %179 = alloca i32, align 4
  %180 = alloca i32, align 4
  %181 = alloca i32, align 4
  %182 = alloca i32, align 4
  %183 = alloca i32, align 4
  %184 = alloca i32, align 4
  %185 = alloca i32, align 4
  %186 = alloca i64, align 8
  %187 = alloca i64, align 8
  %188 = alloca { i32, i32, i32, i32 }, align 8
  %189 = alloca i32, align 4
  %190 = alloca i32, align 4
  %191 = alloca i32, align 4
  %192 = alloca i32, align 4
  %193 = alloca i64, align 8
  %194 = alloca i32, align 4
  %195 = alloca i32, align 4
  %196 = alloca i32, align 4
  %197 = alloca i32, align 4
  %198 = alloca i32, align 4
  %199 = alloca i64, align 8
  %200 = alloca i64, align 8
  %201 = alloca i1, align 1
  %202 = alloca i1, align 1
  %203 = alloca i64, align 8
  %204 = alloca { i32, i32, i32, i32 }, align 8
  %205 = alloca i32, align 4
  %206 = alloca i32, align 4
  %207 = alloca i32, align 4
  %208 = alloca i32, align 4
  %209 = alloca i64, align 8
  %210 = alloca i32, align 4
  %211 = alloca i32, align 4
  %212 = alloca i32, align 4
  %213 = alloca i32, align 4
  %214 = alloca i32, align 4
  %215 = alloca i64, align 8
  %216 = alloca i64, align 8
  %217 = alloca i1, align 1
  %218 = alloca i1, align 1
  %219 = alloca i1, align 1
  %220 = alloca i64, align 8
  %221 = alloca { i32, i32, i32, i32 }, align 8
  %222 = alloca i32, align 4
  %223 = alloca i32, align 4
  %224 = alloca i32, align 4
  %225 = alloca i32, align 4
  %226 = alloca i64, align 8
  %227 = alloca i32, align 4
  %228 = alloca i32, align 4
  %229 = alloca i32, align 4
  %230 = alloca i32, align 4
  %231 = alloca i32, align 4
  %232 = alloca i64, align 8
  %233 = alloca i64, align 8
  %234 = alloca i32, align 4
  %235 = alloca i32, align 4
  %236 = alloca i32, align 4
  %237 = alloca i32, align 4
  %238 = alloca i32, align 4
  %239 = alloca i32, align 4
  %240 = alloca i32, align 4
  %241 = alloca i32, align 4
  %242 = alloca i32, align 4
  %243 = alloca i32, align 4
  %244 = alloca i32, align 4
  %245 = alloca i32, align 4
  %246 = alloca i32, align 4
  %247 = alloca i32, align 4
  %248 = alloca i64, align 8
  %249 = alloca i64, align 8
  %250 = alloca { i32, i32, i32, i32 }, align 8
  %251 = alloca i32, align 4
  %252 = alloca i32, align 4
  %253 = alloca i32, align 4
  %254 = alloca i32, align 4
  %255 = alloca i64, align 8
  %256 = alloca i32, align 4
  %257 = alloca i32, align 4
  %258 = alloca i32, align 4
  %259 = alloca i32, align 4
  %260 = alloca i32, align 4
  %261 = alloca i64, align 8
  %262 = alloca i64, align 8
  %263 = alloca i32, align 4
  %264 = alloca i32, align 4
  %265 = alloca i32, align 4
  %266 = alloca i32, align 4
  %267 = alloca i32, align 4
  %268 = alloca i32, align 4
  %269 = alloca i32, align 4
  %270 = alloca i32, align 4
  %271 = alloca i32, align 4
  %272 = alloca i32, align 4
  %273 = alloca i32, align 4
  %274 = alloca i32, align 4
  %275 = alloca i32, align 4
  %276 = alloca i32, align 4
  %277 = alloca i32, align 4
  %278 = alloca i32, align 4
  %279 = alloca i32, align 4
  %280 = alloca i32, align 4
  %281 = alloca i32, align 4
  %282 = alloca i32, align 4
  %283 = alloca i32, align 4
  %284 = alloca i32, align 4
  %285 = alloca i32, align 4
  %286 = alloca i32, align 4
  %287 = alloca i32, align 4
  %288 = alloca i32, align 4
  %289 = alloca i32, align 4
  %290 = alloca i32, align 4
  %291 = alloca i32, align 4
  %292 = alloca i32, align 4
  %293 = alloca i32, align 4
  %294 = alloca i32, align 4
  %295 = alloca i32, align 4
  %296 = alloca i32, align 4
  %297 = alloca i32, align 4
  %298 = alloca i32, align 4
  %299 = alloca i32, align 4
  %300 = alloca i32, align 4
  %301 = alloca i32, align 4
  %302 = alloca i32, align 4
  %303 = alloca i32, align 4
  %304 = alloca i32, align 4
  %305 = alloca i64, align 8
  %306 = alloca i64, align 8
  %307 = alloca i32, align 4
  %308 = alloca i32, align 4
  %309 = alloca i32, align 4
  %310 = alloca i32, align 4
  %311 = alloca i32, align 4
  %312 = alloca i8*, align 8
  %313 = alloca i32, align 4
  %314 = alloca i32, align 4
  %315 = alloca i32, align 4
  %316 = alloca i32, align 4
  %317 = alloca i32, align 4
  %318 = alloca i8*, align 8
  %319 = alloca i32, align 4
  %320 = alloca i32, align 4
  %321 = alloca i32, align 4
  %322 = alloca i32, align 4
  %323 = alloca i32, align 4
  %324 = alloca i8*, align 8
  %325 = alloca i32, align 4
  %326 = alloca i32, align 4
  %327 = alloca i32, align 4
  %328 = alloca i32, align 4
  %329 = alloca i32, align 4
  %330 = alloca i8*, align 8
  %331 = alloca i32, align 4
  %332 = alloca i32, align 4
  %333 = alloca i32, align 4
  %334 = alloca i32, align 4
  %335 = alloca i32, align 4
  %336 = alloca i8*, align 8
  %337 = alloca i64, align 8
  %338 = alloca i64, align 8
  %339 = alloca { i32, i32, i32, i32 }, align 8
  %340 = alloca i32, align 4
  %341 = alloca i32, align 4
  %342 = alloca i32, align 4
  %343 = alloca i32, align 4
  %344 = alloca i64, align 8
  %345 = alloca i32, align 4
  %346 = alloca i32, align 4
  %347 = alloca i32, align 4
  %348 = alloca i32, align 4
  %349 = alloca i32, align 4
  %350 = alloca i64, align 8
  %351 = alloca i64, align 8
  %352 = alloca i32, align 4
  %353 = alloca i32, align 4
  %354 = alloca i32, align 4
  %355 = alloca i32, align 4
  %356 = alloca i32, align 4
  %357 = alloca i32, align 4
  %358 = alloca i32, align 4
  %359 = alloca i32, align 4
  %360 = alloca i32, align 4
  %361 = alloca i32, align 4
  %362 = alloca i32, align 4
  %363 = alloca i32, align 4
  %364 = alloca i32, align 4
  %365 = alloca i32, align 4
  %366 = alloca i64, align 8
  %367 = alloca { i8*, i8*, i8* }, align 8
  %368 = alloca { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, align 8
  %369 = alloca { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, align 8
  %370 = alloca { i32, i32, i32, i32, i32, i32, i64 }, align 8
  %371 = alloca i64, align 8
  %372 = alloca i32, align 4
  %373 = alloca i32, align 4
  %374 = alloca i32, align 4
  %375 = alloca i32, align 4
  %376 = alloca i32, align 4
  %377 = alloca i32, align 4
  %378 = alloca i32, align 4
  %379 = alloca i32, align 4
  %380 = alloca i32, align 4
  %381 = alloca i32, align 4
  %382 = alloca i32, align 4
  %383 = alloca i32, align 4
  %384 = alloca i32, align 4
  %385 = alloca i32, align 4
  %386 = alloca i32, align 4
  %387 = alloca i32, align 4
  %388 = alloca i32, align 4
  %389 = alloca i8, align 1
  %390 = alloca i8, align 1
  %391 = alloca i8, align 1
  %392 = alloca i8, align 1
  %393 = alloca i32, align 4
  %394 = alloca i32, align 4
  %395 = alloca i32, align 4
  %396 = alloca i32, align 4
  %397 = alloca i64, align 8
  %398 = alloca i32, align 4
  %399 = alloca i32, align 4
  %400 = alloca i32, align 4
  %401 = alloca i32, align 4
  %402 = alloca i64, align 8
  %403 = alloca i64, align 8
  %404 = alloca i64, align 8
  %405 = alloca i64, align 8
  %406 = alloca i64, align 8
  %407 = alloca i32, align 4
  %408 = alloca i64, align 8
  %409 = alloca i32, align 4
  %410 = alloca i32, align 4
  %411 = alloca i64, align 8
  %412 = alloca i32, align 4
  %413 = alloca i32, align 4
  %414 = alloca i32, align 4
  %415 = alloca i32, align 4
  %416 = alloca i32, align 4
  %417 = alloca i8*, align 8
  %418 = alloca i32, align 4
  br label %419

419:                                              ; preds = %alloc
  %SDL_INIT_VIDEO = load i32, i32* @SDL_INIT_VIDEO, align 4
  store i32 %SDL_INIT_VIDEO, i32* %418, align 4
  br label %420

420:                                              ; preds = %419
  %flags = load i32, i32* %418, align 4
  %421 = call i32 @SDL_Init(i32 %flags)
  store i8* getelementptr inbounds ([5 x i8], [5 x i8]* @string, i32 0, i32 0), i8** %417, align 8
  store i32 0, i32* %416, align 4
  store i32 0, i32* %415, align 4
  store i32 800, i32* %414, align 4
  store i32 600, i32* %413, align 4
  store i32 0, i32* %412, align 4
  br label %422

422:                                              ; preds = %420
  %title = load i8*, i8** %417, align 8
  %x = load i32, i32* %416, align 4
  %y = load i32, i32* %415, align 4
  %w = load i32, i32* %414, align 4
  %h = load i32, i32* %413, align 4
  %flags1 = load i32, i32* %412, align 4
  %423 = call i64 @SDL_CreateWindow(i8* %title, i32 %x, i32 %y, i32 %w, i32 %h, i32 %flags1)
  store i64 %423, i64* %411, align 4
  store i32 0, i32* %410, align 4
  store i32 0, i32* %409, align 4
  br label %424

424:                                              ; preds = %422
  %window = load i64, i64* %411, align 4
  %index = load i32, i32* %410, align 4
  %flags2 = load i32, i32* %409, align 4
  %425 = call i64 @SDL_CreateRenderer(i64 %window, i32 %index, i32 %flags2)
  store i64 %425, i64* %408, align 4
  br label %426

426:                                              ; preds = %424
  store i32 0, i32* %407, align 4
  br label %427

427:                                              ; preds = %426
  br label %428

428:                                              ; preds = %708, %706, %704, %427
  %renderer = load i64, i64* %408, align 4
  store i64 %renderer, i64* %406, align 4
  %renderer3 = load i64, i64* %408, align 4
  store i64 %renderer3, i64* %405, align 4
  %renderer4 = load i64, i64* %408, align 4
  store i64 %renderer4, i64* %404, align 4
  %renderer5 = load i64, i64* %408, align 4
  store i64 %renderer5, i64* %403, align 4
  br label %429

429:                                              ; preds = %428
  %renderer6 = load i64, i64* %403, align 4
  store i64 %renderer6, i64* %402, align 4
  store i32 0, i32* %401, align 4
  store i32 0, i32* %400, align 4
  store i32 0, i32* %399, align 4
  store i32 255, i32* %398, align 4
  br label %430

430:                                              ; preds = %429
  %renderer7 = load i64, i64* %402, align 4
  %r = load i32, i32* %401, align 4
  %g = load i32, i32* %400, align 4
  %b = load i32, i32* %399, align 4
  %a = load i32, i32* %398, align 4
  %431 = call i32 @SDL_SetRenderDrawColor(i64 %renderer7, i32 %r, i32 %g, i32 %b, i32 %a)
  %renderer8 = load i64, i64* %403, align 4
  store i64 %renderer8, i64* %397, align 4
  br label %432

432:                                              ; preds = %430
  %renderer9 = load i64, i64* %397, align 4
  %433 = call i32 @SDL_RenderClear(i64 %renderer9)
  br label %434

434:                                              ; preds = %432
  br label %435

435:                                              ; preds = %434
  br label %436

436:                                              ; preds = %435
  store i32 0, i32* %396, align 4
  store i32 0, i32* %395, align 4
  store i32 0, i32* %394, align 4
  store i32 0, i32* %393, align 4
  store i8 0, i8* %392, align 1
  store i8 0, i8* %391, align 1
  store i8 0, i8* %390, align 1
  store i8 0, i8* %389, align 1
  store i32 0, i32* %388, align 4
  store i32 0, i32* %387, align 4
  store i32 0, i32* %386, align 4
  store i32 0, i32* %385, align 4
  store i32 0, i32* %384, align 4
  store i32 0, i32* %383, align 4
  store i32 0, i32* %382, align 4
  store i32 0, i32* %381, align 4
  store i32 0, i32* %380, align 4
  store i32 0, i32* %379, align 4
  store i32 0, i32* %378, align 4
  store i32 0, i32* %377, align 4
  store i32 0, i32* %376, align 4
  store i32 0, i32* %375, align 4
  store i32 0, i32* %374, align 4
  store i32 0, i32* %373, align 4
  store i32 0, i32* %372, align 4
  store i64 0, i64* %371, align 4
  br label %437

437:                                              ; preds = %436
  br label %438

438:                                              ; preds = %437
  %type = load i32, i32* %377, align 4
  %timestamp = load i32, i32* %376, align 4
  %windowID = load i32, i32* %375, align 4
  %padding = load i32, i32* %374, align 4
  %scancode = load i32, i32* %373, align 4
  %sym = load i32, i32* %372, align 4
  %padding10 = load i64, i64* %371, align 4
  %gep = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %370, i32 0, i32 0
  store i32 %type, i32* %gep, align 4
  %gep11 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %370, i32 0, i32 1
  store i32 %timestamp, i32* %gep11, align 4
  %gep12 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %370, i32 0, i32 2
  store i32 %windowID, i32* %gep12, align 4
  %gep13 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %370, i32 0, i32 3
  store i32 %padding, i32* %gep13, align 4
  %gep14 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %370, i32 0, i32 4
  store i32 %scancode, i32* %gep14, align 4
  %gep15 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %370, i32 0, i32 5
  store i32 %sym, i32* %gep15, align 4
  %gep16 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %370, i32 0, i32 6
  store i64 %padding10, i64* %gep16, align 4
  %cast = bitcast { i32, i32, i32, i32, i32, i32, i64 }* %370 to i8*
  %type17 = load i32, i32* %386, align 4
  %timestamp18 = load i32, i32* %385, align 4
  %windowID19 = load i32, i32* %384, align 4
  %which = load i32, i32* %383, align 4
  %state = load i32, i32* %382, align 4
  %x20 = load i32, i32* %381, align 4
  %y21 = load i32, i32* %380, align 4
  %xrel = load i32, i32* %379, align 4
  %yrel = load i32, i32* %378, align 4
  %gep22 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %369, i32 0, i32 0
  store i32 %type17, i32* %gep22, align 4
  %gep23 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %369, i32 0, i32 1
  store i32 %timestamp18, i32* %gep23, align 4
  %gep24 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %369, i32 0, i32 2
  store i32 %windowID19, i32* %gep24, align 4
  %gep25 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %369, i32 0, i32 3
  store i32 %which, i32* %gep25, align 4
  %gep26 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %369, i32 0, i32 4
  store i32 %state, i32* %gep26, align 4
  %gep27 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %369, i32 0, i32 5
  store i32 %x20, i32* %gep27, align 4
  %gep28 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %369, i32 0, i32 6
  store i32 %y21, i32* %gep28, align 4
  %gep29 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %369, i32 0, i32 7
  store i32 %xrel, i32* %gep29, align 4
  %gep30 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %369, i32 0, i32 8
  store i32 %yrel, i32* %gep30, align 4
  %cast31 = bitcast { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %369 to i8*
  %type32 = load i32, i32* %396, align 4
  %timestamp33 = load i32, i32* %395, align 4
  %windowId = load i32, i32* %394, align 4
  %which34 = load i32, i32* %393, align 4
  %button = load i8, i8* %392, align 1
  %state35 = load i8, i8* %391, align 1
  %clicks = load i8, i8* %390, align 1
  %padding36 = load i8, i8* %389, align 1
  %x37 = load i32, i32* %388, align 4
  %y38 = load i32, i32* %387, align 4
  %gep39 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %368, i32 0, i32 0
  store i32 %type32, i32* %gep39, align 4
  %gep40 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %368, i32 0, i32 1
  store i32 %timestamp33, i32* %gep40, align 4
  %gep41 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %368, i32 0, i32 2
  store i32 %windowId, i32* %gep41, align 4
  %gep42 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %368, i32 0, i32 3
  store i32 %which34, i32* %gep42, align 4
  %gep43 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %368, i32 0, i32 4
  store i8 %button, i8* %gep43, align 1
  %gep44 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %368, i32 0, i32 5
  store i8 %state35, i8* %gep44, align 1
  %gep45 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %368, i32 0, i32 6
  store i8 %clicks, i8* %gep45, align 1
  %gep46 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %368, i32 0, i32 7
  store i8 %padding36, i8* %gep46, align 1
  %gep47 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %368, i32 0, i32 8
  store i32 %x37, i32* %gep47, align 4
  %gep48 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %368, i32 0, i32 9
  store i32 %y38, i32* %gep48, align 4
  %cast49 = bitcast { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %368 to i8*
  %gep50 = getelementptr inbounds { i8*, i8*, i8* }, { i8*, i8*, i8* }* %367, i32 0, i32 0
  store i8* %cast, i8** %gep50, align 8
  %gep51 = getelementptr inbounds { i8*, i8*, i8* }, { i8*, i8*, i8* }* %367, i32 0, i32 1
  store i8* %cast31, i8** %gep51, align 8
  %gep52 = getelementptr inbounds { i8*, i8*, i8* }, { i8*, i8*, i8* }* %367, i32 0, i32 2
  store i8* %cast49, i8** %gep52, align 8
  %cast53 = bitcast { i8*, i8*, i8* }* %367 to i8*
  %439 = call i8* @SDL_WaitEventEx(i8* %cast53)
  %cast54 = bitcast i8* %439 to { i8*, i8*, i8* }*
  %key = getelementptr inbounds { i8*, i8*, i8* }, { i8*, i8*, i8* }* %cast54, i32 0, i32 0
  %key55 = load i8*, i8** %key, align 8
  %cast56 = bitcast i8* %key55 to { i32, i32, i32, i32, i32, i32, i64 }*
  %type57 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %cast56, i32 0, i32 0
  %type58 = load i32, i32* %type57, align 4
  %timestamp59 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %cast56, i32 0, i32 1
  %timestamp60 = load i32, i32* %timestamp59, align 4
  %windowID61 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %cast56, i32 0, i32 2
  %windowID62 = load i32, i32* %windowID61, align 4
  %padding63 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %cast56, i32 0, i32 3
  %padding64 = load i32, i32* %padding63, align 4
  %scancode65 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %cast56, i32 0, i32 4
  %scancode66 = load i32, i32* %scancode65, align 4
  %sym67 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %cast56, i32 0, i32 5
  %sym68 = load i32, i32* %sym67, align 4
  %padding69 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i64 }, { i32, i32, i32, i32, i32, i32, i64 }* %cast56, i32 0, i32 6
  %padding70 = load i64, i64* %padding69, align 4
  %motion = getelementptr inbounds { i8*, i8*, i8* }, { i8*, i8*, i8* }* %cast54, i32 0, i32 1
  %motion71 = load i8*, i8** %motion, align 8
  %cast72 = bitcast i8* %motion71 to { i32, i32, i32, i32, i32, i32, i32, i32, i32 }*
  %type73 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %cast72, i32 0, i32 0
  %type74 = load i32, i32* %type73, align 4
  %timestamp75 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %cast72, i32 0, i32 1
  %timestamp76 = load i32, i32* %timestamp75, align 4
  %windowID77 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %cast72, i32 0, i32 2
  %windowID78 = load i32, i32* %windowID77, align 4
  %which79 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %cast72, i32 0, i32 3
  %which80 = load i32, i32* %which79, align 4
  %state81 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %cast72, i32 0, i32 4
  %state82 = load i32, i32* %state81, align 4
  %x83 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %cast72, i32 0, i32 5
  %x84 = load i32, i32* %x83, align 4
  %y85 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %cast72, i32 0, i32 6
  %y86 = load i32, i32* %y85, align 4
  %xrel87 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %cast72, i32 0, i32 7
  %xrel88 = load i32, i32* %xrel87, align 4
  %yrel89 = getelementptr inbounds { i32, i32, i32, i32, i32, i32, i32, i32, i32 }, { i32, i32, i32, i32, i32, i32, i32, i32, i32 }* %cast72, i32 0, i32 8
  %yrel90 = load i32, i32* %yrel89, align 4
  %button91 = getelementptr inbounds { i8*, i8*, i8* }, { i8*, i8*, i8* }* %cast54, i32 0, i32 2
  %button92 = load i8*, i8** %button91, align 8
  %cast93 = bitcast i8* %button92 to { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }*
  %type94 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %cast93, i32 0, i32 0
  %type95 = load i32, i32* %type94, align 4
  %timestamp96 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %cast93, i32 0, i32 1
  %timestamp97 = load i32, i32* %timestamp96, align 4
  %windowId98 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %cast93, i32 0, i32 2
  %windowId99 = load i32, i32* %windowId98, align 4
  %which100 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %cast93, i32 0, i32 3
  %which101 = load i32, i32* %which100, align 4
  %button102 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %cast93, i32 0, i32 4
  %button103 = load i8, i8* %button102, align 1
  %state104 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %cast93, i32 0, i32 5
  %state105 = load i8, i8* %state104, align 1
  %clicks106 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %cast93, i32 0, i32 6
  %clicks107 = load i8, i8* %clicks106, align 1
  %padding108 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %cast93, i32 0, i32 7
  %padding109 = load i8, i8* %padding108, align 1
  %x110 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %cast93, i32 0, i32 8
  %x111 = load i32, i32* %x110, align 4
  %y112 = getelementptr inbounds { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }, { i32, i32, i32, i32, i8, i8, i8, i8, i32, i32 }* %cast93, i32 0, i32 9
  %y113 = load i32, i32* %y112, align 4
  br label %440

440:                                              ; preds = %438
  br label %441

441:                                              ; preds = %440
  br label %442

442:                                              ; preds = %441
  store i64 0, i64* %366, align 4
  br label %443

443:                                              ; preds = %442
  store i32 252, i32* %365, align 4
  store i32 252, i32* %364, align 4
  store i32 252, i32* %363, align 4
  br label %444

444:                                              ; preds = %443
  %b114 = load i32, i32* %363, align 4
  %g115 = load i32, i32* %364, align 4
  %r116 = load i32, i32* %365, align 4
  store i32 %r116, i32* %362, align 4
  store i32 %g115, i32* %361, align 4
  store i32 %b114, i32* %360, align 4
  br label %445

445:                                              ; preds = %444
  store i32 0, i32* %359, align 4
  store i32 0, i32* %358, align 4
  store i32 0, i32* %357, align 4
  store i32 10, i32* %356, align 4
  store i32 10, i32* %355, align 4
  store i32 250, i32* %354, align 4
  store i32 150, i32* %353, align 4
  store i32 0, i32* %352, align 4
  br label %446

446:                                              ; preds = %445
  br label %447

447:                                              ; preds = %446
  br label %448

448:                                              ; preds = %447
  %renderer117 = load i64, i64* %404, align 4
  store i64 %renderer117, i64* %351, align 4
  br label %449

449:                                              ; preds = %448
  %renderer118 = load i64, i64* %351, align 4
  store i64 %renderer118, i64* %350, align 4
  %color.r = load i32, i32* %362, align 4
  store i32 %color.r, i32* %349, align 4
  %color.g = load i32, i32* %361, align 4
  store i32 %color.g, i32* %348, align 4
  %color.b = load i32, i32* %360, align 4
  store i32 %color.b, i32* %347, align 4
  store i32 255, i32* %346, align 4
  br label %450

450:                                              ; preds = %449
  %renderer119 = load i64, i64* %350, align 4
  %r120 = load i32, i32* %349, align 4
  %g121 = load i32, i32* %348, align 4
  %b122 = load i32, i32* %347, align 4
  %a123 = load i32, i32* %346, align 4
  %451 = call i32 @SDL_SetRenderDrawColor(i64 %renderer119, i32 %r120, i32 %g121, i32 %b122, i32 %a123)
  store i32 %451, i32* %345, align 4
  %452 = load i32, i32* %345, align 4
  br label %453

453:                                              ; preds = %450
  %renderer124 = load i64, i64* %404, align 4
  store i64 %renderer124, i64* %344, align 4
  br label %454

454:                                              ; preds = %453
  %item.height = load i32, i32* %353, align 4
  %item.width = load i32, i32* %354, align 4
  %item.y = load i32, i32* %355, align 4
  %item.x = load i32, i32* %356, align 4
  store i32 %item.x, i32* %343, align 4
  store i32 %item.y, i32* %342, align 4
  store i32 %item.width, i32* %341, align 4
  store i32 %item.height, i32* %340, align 4
  br label %455

455:                                              ; preds = %454
  br label %456

456:                                              ; preds = %455
  %renderer125 = load i64, i64* %344, align 4
  %x126 = load i32, i32* %343, align 4
  %y127 = load i32, i32* %342, align 4
  %width = load i32, i32* %341, align 4
  %height = load i32, i32* %340, align 4
  %gep128 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %339, i32 0, i32 0
  store i32 %x126, i32* %gep128, align 4
  %gep129 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %339, i32 0, i32 1
  store i32 %y127, i32* %gep129, align 4
  %gep130 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %339, i32 0, i32 2
  store i32 %width, i32* %gep130, align 4
  %gep131 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %339, i32 0, i32 3
  store i32 %height, i32* %gep131, align 4
  %cast132 = bitcast { i32, i32, i32, i32 }* %339 to i8*
  %457 = call i32 @SDL_RenderFillRect(i64 %renderer125, i8* %cast132)
  %renderer133 = load i64, i64* %404, align 4
  store i64 %renderer133, i64* %338, align 4
  store i64 0, i64* %337, align 4
  br label %458

458:                                              ; preds = %693, %690, %687, %456
  %state.counter = load i32, i32* %407, align 4
  %459 = add i32 30, %state.counter
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @string.1, i32 0, i32 0), i8** %336, align 8
  store i32 %459, i32* %335, align 4
  store i32 30, i32* %334, align 4
  store i32 210, i32* %333, align 4
  store i32 30, i32* %332, align 4
  store i32 0, i32* %331, align 4
  %index134 = load i64, i64* %337, align 4
  %460 = icmp eq i64 %index134, 0
  br i1 %460, label %467, label %461

461:                                              ; preds = %458
  %state.counter135 = load i32, i32* %407, align 4
  %462 = add i32 30, %state.counter135
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @string.2, i32 0, i32 0), i8** %330, align 8
  store i32 %462, i32* %329, align 4
  store i32 70, i32* %328, align 4
  store i32 210, i32* %327, align 4
  store i32 30, i32* %326, align 4
  store i32 0, i32* %325, align 4
  %index136 = load i64, i64* %337, align 4
  %463 = icmp eq i64 %index136, 1
  br i1 %463, label %468, label %464

464:                                              ; preds = %461
  %state.counter137 = load i32, i32* %407, align 4
  %465 = add i32 30, %state.counter137
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @string.3, i32 0, i32 0), i8** %324, align 8
  store i32 %465, i32* %323, align 4
  store i32 110, i32* %322, align 4
  store i32 70, i32* %321, align 4
  store i32 30, i32* %320, align 4
  store i32 0, i32* %319, align 4
  %index138 = load i64, i64* %337, align 4
  %466 = icmp eq i64 %index138, 2
  br i1 %466, label %469, label %470

467:                                              ; preds = %458
  %load = load i8*, i8** %336, align 8
  store i8* %load, i8** %312, align 8
  %load141 = load i32, i32* %335, align 4
  store i32 %load141, i32* %311, align 4
  %load144 = load i32, i32* %334, align 4
  store i32 %load144, i32* %310, align 4
  %load147 = load i32, i32* %333, align 4
  store i32 %load147, i32* %309, align 4
  %load150 = load i32, i32* %332, align 4
  store i32 %load150, i32* %308, align 4
  %load153 = load i32, i32* %331, align 4
  store i32 %load153, i32* %307, align 4
  br label %junction

468:                                              ; preds = %461
  %load139 = load i8*, i8** %330, align 8
  store i8* %load139, i8** %312, align 8
  %load142 = load i32, i32* %329, align 4
  store i32 %load142, i32* %311, align 4
  %load145 = load i32, i32* %328, align 4
  store i32 %load145, i32* %310, align 4
  %load148 = load i32, i32* %327, align 4
  store i32 %load148, i32* %309, align 4
  %load151 = load i32, i32* %326, align 4
  store i32 %load151, i32* %308, align 4
  %load154 = load i32, i32* %325, align 4
  store i32 %load154, i32* %307, align 4
  br label %junction

469:                                              ; preds = %464
  %load140 = load i8*, i8** %324, align 8
  store i8* %load140, i8** %312, align 8
  %load143 = load i32, i32* %323, align 4
  store i32 %load143, i32* %311, align 4
  %load146 = load i32, i32* %322, align 4
  store i32 %load146, i32* %310, align 4
  %load149 = load i32, i32* %321, align 4
  store i32 %load149, i32* %309, align 4
  %load152 = load i32, i32* %320, align 4
  store i32 %load152, i32* %308, align 4
  %load155 = load i32, i32* %319, align 4
  store i32 %load155, i32* %307, align 4
  br label %junction

470:                                              ; preds = %464
  %state.counter156 = load i32, i32* %407, align 4
  %471 = add i32 140, %state.counter156
  store i8* getelementptr inbounds ([10 x i8], [10 x i8]* @string.4, i32 0, i32 0), i8** %318, align 8
  store i32 %471, i32* %317, align 4
  store i32 110, i32* %316, align 4
  store i32 70, i32* %315, align 4
  store i32 30, i32* %314, align 4
  store i32 0, i32* %313, align 4
  br label %472

472:                                              ; preds = %470
  %renderer157 = load i64, i64* %338, align 4
  store i64 %renderer157, i64* %306, align 4
  br label %473

473:                                              ; preds = %472
  %renderer158 = load i64, i64* %306, align 4
  store i64 %renderer158, i64* %305, align 4
  br label %474

474:                                              ; preds = %473
  store i32 41, i32* %304, align 4
  store i32 128, i32* %303, align 4
  store i32 185, i32* %302, align 4
  br label %475

475:                                              ; preds = %474
  %b159 = load i32, i32* %302, align 4
  %g160 = load i32, i32* %303, align 4
  %r161 = load i32, i32* %304, align 4
  store i32 %r161, i32* %301, align 4
  store i32 %g160, i32* %300, align 4
  store i32 %b159, i32* %299, align 4
  br label %476

476:                                              ; preds = %475
  %button.height = load i32, i32* %314, align 4
  %477 = add i32 %button.height, 2
  %button.width = load i32, i32* %315, align 4
  %button.y = load i32, i32* %316, align 4
  %button.x = load i32, i32* %317, align 4
  store i32 0, i32* %298, align 4
  store i32 0, i32* %297, align 4
  store i32 0, i32* %296, align 4
  store i32 %button.x, i32* %295, align 4
  store i32 %button.y, i32* %294, align 4
  store i32 %button.width, i32* %293, align 4
  store i32 %477, i32* %292, align 4
  store i32 0, i32* %291, align 4
  store i32 41, i32* %290, align 4
  store i32 128, i32* %289, align 4
  store i32 185, i32* %288, align 4
  br label %478

478:                                              ; preds = %476
  %b162 = load i32, i32* %288, align 4
  %g163 = load i32, i32* %289, align 4
  %r164 = load i32, i32* %290, align 4
  store i32 %r164, i32* %287, align 4
  store i32 %g163, i32* %286, align 4
  store i32 %b162, i32* %285, align 4
  br label %479

479:                                              ; preds = %478
  %button.height165 = load i32, i32* %314, align 4
  %480 = add i32 %button.height165, 2
  %button.width166 = load i32, i32* %315, align 4
  %button.y167 = load i32, i32* %316, align 4
  %button.x168 = load i32, i32* %317, align 4
  store i32 0, i32* %284, align 4
  store i32 0, i32* %283, align 4
  store i32 0, i32* %282, align 4
  store i32 %button.x168, i32* %281, align 4
  store i32 %button.y167, i32* %280, align 4
  store i32 %button.width166, i32* %279, align 4
  store i32 %480, i32* %278, align 4
  store i32 0, i32* %277, align 4
  store i32 41, i32* %276, align 4
  store i32 128, i32* %275, align 4
  store i32 185, i32* %274, align 4
  br label %481

481:                                              ; preds = %479
  %b169 = load i32, i32* %274, align 4
  %g170 = load i32, i32* %275, align 4
  %r171 = load i32, i32* %276, align 4
  store i32 %r171, i32* %273, align 4
  store i32 %g170, i32* %272, align 4
  store i32 %b169, i32* %271, align 4
  br label %482

482:                                              ; preds = %481
  %button.height172 = load i32, i32* %314, align 4
  %483 = add i32 %button.height172, 2
  %button.width173 = load i32, i32* %315, align 4
  %button.y174 = load i32, i32* %316, align 4
  %button.x175 = load i32, i32* %317, align 4
  store i32 0, i32* %270, align 4
  store i32 0, i32* %269, align 4
  store i32 0, i32* %268, align 4
  store i32 %button.x175, i32* %267, align 4
  store i32 %button.y174, i32* %266, align 4
  store i32 %button.width173, i32* %265, align 4
  store i32 %483, i32* %264, align 4
  store i32 0, i32* %263, align 4
  br label %484

484:                                              ; preds = %482
  br label %condition

condition:                                        ; preds = %484
  %MouseOver = load i32, i32* @MouseOver, align 4
  %item.state = load i32, i32* %313, align 4
  %485 = icmp eq i32 %item.state, %MouseOver
  br i1 %485, label %486, label %condition177

486:                                              ; preds = %condition
  br label %490

condition177:                                     ; preds = %condition
  %Pressed = load i32, i32* @Pressed, align 4
  %item.state176 = load i32, i32* %313, align 4
  %487 = icmp eq i32 %item.state176, %Pressed
  br i1 %487, label %488, label %489

488:                                              ; preds = %condition177
  br label %522

489:                                              ; preds = %condition177
  br label %538

490:                                              ; preds = %486
  br label %491

491:                                              ; preds = %490
  %renderer178 = load i64, i64* %305, align 4
  store i64 %renderer178, i64* %262, align 4
  br label %492

492:                                              ; preds = %491
  %renderer179 = load i64, i64* %262, align 4
  store i64 %renderer179, i64* %261, align 4
  %color.r180 = load i32, i32* %287, align 4
  store i32 %color.r180, i32* %260, align 4
  %color.g181 = load i32, i32* %286, align 4
  store i32 %color.g181, i32* %259, align 4
  %color.b182 = load i32, i32* %285, align 4
  store i32 %color.b182, i32* %258, align 4
  store i32 255, i32* %257, align 4
  br label %493

493:                                              ; preds = %492
  %renderer183 = load i64, i64* %261, align 4
  %r184 = load i32, i32* %260, align 4
  %g185 = load i32, i32* %259, align 4
  %b186 = load i32, i32* %258, align 4
  %a187 = load i32, i32* %257, align 4
  %494 = call i32 @SDL_SetRenderDrawColor(i64 %renderer183, i32 %r184, i32 %g185, i32 %b186, i32 %a187)
  store i32 %494, i32* %256, align 4
  %495 = load i32, i32* %256, align 4
  br label %496

496:                                              ; preds = %493
  %renderer188 = load i64, i64* %305, align 4
  store i64 %renderer188, i64* %255, align 4
  br label %497

497:                                              ; preds = %496
  %item.height189 = load i32, i32* %278, align 4
  %item.width190 = load i32, i32* %279, align 4
  %item.y191 = load i32, i32* %280, align 4
  %item.x192 = load i32, i32* %281, align 4
  store i32 %item.x192, i32* %254, align 4
  store i32 %item.y191, i32* %253, align 4
  store i32 %item.width190, i32* %252, align 4
  store i32 %item.height189, i32* %251, align 4
  br label %498

498:                                              ; preds = %497
  br label %499

499:                                              ; preds = %498
  %renderer193 = load i64, i64* %255, align 4
  %x194 = load i32, i32* %254, align 4
  %y195 = load i32, i32* %253, align 4
  %width196 = load i32, i32* %252, align 4
  %height197 = load i32, i32* %251, align 4
  %gep198 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %250, i32 0, i32 0
  store i32 %x194, i32* %gep198, align 4
  %gep199 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %250, i32 0, i32 1
  store i32 %y195, i32* %gep199, align 4
  %gep200 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %250, i32 0, i32 2
  store i32 %width196, i32* %gep200, align 4
  %gep201 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %250, i32 0, i32 3
  store i32 %height197, i32* %gep201, align 4
  %cast202 = bitcast { i32, i32, i32, i32 }* %250 to i8*
  %500 = call i32 @SDL_RenderFillRect(i64 %renderer193, i8* %cast202)
  %renderer203 = load i64, i64* %305, align 4
  store i64 %renderer203, i64* %249, align 4
  store i64 0, i64* %248, align 4
  br label %501

501:                                              ; preds = %499
  store i32 61, i32* %247, align 4
  store i32 174, i32* %246, align 4
  store i32 233, i32* %245, align 4
  br label %502

502:                                              ; preds = %501
  %b204 = load i32, i32* %245, align 4
  %g205 = load i32, i32* %246, align 4
  %r206 = load i32, i32* %247, align 4
  store i32 %r206, i32* %244, align 4
  store i32 %g205, i32* %243, align 4
  store i32 %b204, i32* %242, align 4
  br label %503

503:                                              ; preds = %502
  %button.height207 = load i32, i32* %314, align 4
  %button.width208 = load i32, i32* %315, align 4
  %button.y209 = load i32, i32* %316, align 4
  %button.x210 = load i32, i32* %317, align 4
  store i32 0, i32* %241, align 4
  store i32 0, i32* %240, align 4
  store i32 0, i32* %239, align 4
  store i32 %button.x210, i32* %238, align 4
  store i32 %button.y209, i32* %237, align 4
  store i32 %button.width208, i32* %236, align 4
  store i32 %button.height207, i32* %235, align 4
  store i32 0, i32* %234, align 4
  %504 = load i64, i64* %249, align 4
  store i64 %504, i64* %305, align 4
  br label %505

505:                                              ; preds = %503
  %renderer211 = load i64, i64* %249, align 4
  store i64 %renderer211, i64* %233, align 4
  br label %506

506:                                              ; preds = %505
  %renderer212 = load i64, i64* %233, align 4
  store i64 %renderer212, i64* %232, align 4
  %color.r213 = load i32, i32* %244, align 4
  store i32 %color.r213, i32* %231, align 4
  %color.g214 = load i32, i32* %243, align 4
  store i32 %color.g214, i32* %230, align 4
  %color.b215 = load i32, i32* %242, align 4
  store i32 %color.b215, i32* %229, align 4
  store i32 255, i32* %228, align 4
  br label %507

507:                                              ; preds = %506
  %renderer216 = load i64, i64* %232, align 4
  %r217 = load i32, i32* %231, align 4
  %g218 = load i32, i32* %230, align 4
  %b219 = load i32, i32* %229, align 4
  %a220 = load i32, i32* %228, align 4
  %508 = call i32 @SDL_SetRenderDrawColor(i64 %renderer216, i32 %r217, i32 %g218, i32 %b219, i32 %a220)
  store i32 %508, i32* %227, align 4
  %509 = load i32, i32* %227, align 4
  br label %510

510:                                              ; preds = %507
  %renderer221 = load i64, i64* %249, align 4
  store i64 %renderer221, i64* %226, align 4
  br label %511

511:                                              ; preds = %510
  %item.height222 = load i32, i32* %235, align 4
  %item.width223 = load i32, i32* %236, align 4
  %item.y224 = load i32, i32* %237, align 4
  %item.x225 = load i32, i32* %238, align 4
  store i32 %item.x225, i32* %225, align 4
  store i32 %item.y224, i32* %224, align 4
  store i32 %item.width223, i32* %223, align 4
  store i32 %item.height222, i32* %222, align 4
  br label %512

512:                                              ; preds = %511
  br label %513

513:                                              ; preds = %512
  %renderer226 = load i64, i64* %226, align 4
  %x227 = load i32, i32* %225, align 4
  %y228 = load i32, i32* %224, align 4
  %width229 = load i32, i32* %223, align 4
  %height230 = load i32, i32* %222, align 4
  %gep231 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %221, i32 0, i32 0
  store i32 %x227, i32* %gep231, align 4
  %gep232 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %221, i32 0, i32 1
  store i32 %y228, i32* %gep232, align 4
  %gep233 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %221, i32 0, i32 2
  store i32 %width229, i32* %gep233, align 4
  %gep234 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %221, i32 0, i32 3
  store i32 %height230, i32* %gep234, align 4
  %cast235 = bitcast { i32, i32, i32, i32 }* %221 to i8*
  %514 = call i32 @SDL_RenderFillRect(i64 %renderer226, i8* %cast235)
  %renderer236 = load i64, i64* %249, align 4
  store i64 %renderer236, i64* %220, align 4
  br label %515

515:                                              ; preds = %513
  store i1 false, i1* %219, align 1
  %516 = load i1, i1* %219, align 1
  br label %517

517:                                              ; preds = %515
  store i1 %516, i1* %218, align 1
  %518 = load i1, i1* %218, align 1
  br label %519

519:                                              ; preds = %517
  store i1 %518, i1* %217, align 1
  %520 = load i1, i1* %217, align 1
  br label %521

521:                                              ; preds = %519
  br label %570

522:                                              ; preds = %488
  br label %523

523:                                              ; preds = %522
  %renderer237 = load i64, i64* %305, align 4
  store i64 %renderer237, i64* %216, align 4
  br label %524

524:                                              ; preds = %523
  %renderer238 = load i64, i64* %216, align 4
  store i64 %renderer238, i64* %215, align 4
  %color.r239 = load i32, i32* %301, align 4
  store i32 %color.r239, i32* %214, align 4
  %color.g240 = load i32, i32* %300, align 4
  store i32 %color.g240, i32* %213, align 4
  %color.b241 = load i32, i32* %299, align 4
  store i32 %color.b241, i32* %212, align 4
  store i32 255, i32* %211, align 4
  br label %525

525:                                              ; preds = %524
  %renderer242 = load i64, i64* %215, align 4
  %r243 = load i32, i32* %214, align 4
  %g244 = load i32, i32* %213, align 4
  %b245 = load i32, i32* %212, align 4
  %a246 = load i32, i32* %211, align 4
  %526 = call i32 @SDL_SetRenderDrawColor(i64 %renderer242, i32 %r243, i32 %g244, i32 %b245, i32 %a246)
  store i32 %526, i32* %210, align 4
  %527 = load i32, i32* %210, align 4
  br label %528

528:                                              ; preds = %525
  %renderer247 = load i64, i64* %305, align 4
  store i64 %renderer247, i64* %209, align 4
  br label %529

529:                                              ; preds = %528
  %item.height248 = load i32, i32* %292, align 4
  %item.width249 = load i32, i32* %293, align 4
  %item.y250 = load i32, i32* %294, align 4
  %item.x251 = load i32, i32* %295, align 4
  store i32 %item.x251, i32* %208, align 4
  store i32 %item.y250, i32* %207, align 4
  store i32 %item.width249, i32* %206, align 4
  store i32 %item.height248, i32* %205, align 4
  br label %530

530:                                              ; preds = %529
  br label %531

531:                                              ; preds = %530
  %renderer252 = load i64, i64* %209, align 4
  %x253 = load i32, i32* %208, align 4
  %y254 = load i32, i32* %207, align 4
  %width255 = load i32, i32* %206, align 4
  %height256 = load i32, i32* %205, align 4
  %gep257 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %204, i32 0, i32 0
  store i32 %x253, i32* %gep257, align 4
  %gep258 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %204, i32 0, i32 1
  store i32 %y254, i32* %gep258, align 4
  %gep259 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %204, i32 0, i32 2
  store i32 %width255, i32* %gep259, align 4
  %gep260 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %204, i32 0, i32 3
  store i32 %height256, i32* %gep260, align 4
  %cast261 = bitcast { i32, i32, i32, i32 }* %204 to i8*
  %532 = call i32 @SDL_RenderFillRect(i64 %renderer252, i8* %cast261)
  %renderer262 = load i64, i64* %305, align 4
  store i64 %renderer262, i64* %203, align 4
  br label %533

533:                                              ; preds = %531
  store i1 false, i1* %202, align 1
  %534 = load i1, i1* %202, align 1
  br label %535

535:                                              ; preds = %533
  store i1 %534, i1* %201, align 1
  %536 = load i1, i1* %201, align 1
  br label %537

537:                                              ; preds = %535
  br label %571

538:                                              ; preds = %489
  br label %539

539:                                              ; preds = %538
  %renderer263 = load i64, i64* %305, align 4
  store i64 %renderer263, i64* %200, align 4
  br label %540

540:                                              ; preds = %539
  %renderer264 = load i64, i64* %200, align 4
  store i64 %renderer264, i64* %199, align 4
  %color.r265 = load i32, i32* %273, align 4
  store i32 %color.r265, i32* %198, align 4
  %color.g266 = load i32, i32* %272, align 4
  store i32 %color.g266, i32* %197, align 4
  %color.b267 = load i32, i32* %271, align 4
  store i32 %color.b267, i32* %196, align 4
  store i32 255, i32* %195, align 4
  br label %541

541:                                              ; preds = %540
  %renderer268 = load i64, i64* %199, align 4
  %r269 = load i32, i32* %198, align 4
  %g270 = load i32, i32* %197, align 4
  %b271 = load i32, i32* %196, align 4
  %a272 = load i32, i32* %195, align 4
  %542 = call i32 @SDL_SetRenderDrawColor(i64 %renderer268, i32 %r269, i32 %g270, i32 %b271, i32 %a272)
  store i32 %542, i32* %194, align 4
  %543 = load i32, i32* %194, align 4
  br label %544

544:                                              ; preds = %541
  %renderer273 = load i64, i64* %305, align 4
  store i64 %renderer273, i64* %193, align 4
  br label %545

545:                                              ; preds = %544
  %item.height274 = load i32, i32* %264, align 4
  %item.width275 = load i32, i32* %265, align 4
  %item.y276 = load i32, i32* %266, align 4
  %item.x277 = load i32, i32* %267, align 4
  store i32 %item.x277, i32* %192, align 4
  store i32 %item.y276, i32* %191, align 4
  store i32 %item.width275, i32* %190, align 4
  store i32 %item.height274, i32* %189, align 4
  br label %546

546:                                              ; preds = %545
  br label %547

547:                                              ; preds = %546
  %renderer278 = load i64, i64* %193, align 4
  %x279 = load i32, i32* %192, align 4
  %y280 = load i32, i32* %191, align 4
  %width281 = load i32, i32* %190, align 4
  %height282 = load i32, i32* %189, align 4
  %gep283 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %188, i32 0, i32 0
  store i32 %x279, i32* %gep283, align 4
  %gep284 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %188, i32 0, i32 1
  store i32 %y280, i32* %gep284, align 4
  %gep285 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %188, i32 0, i32 2
  store i32 %width281, i32* %gep285, align 4
  %gep286 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %188, i32 0, i32 3
  store i32 %height282, i32* %gep286, align 4
  %cast287 = bitcast { i32, i32, i32, i32 }* %188 to i8*
  %548 = call i32 @SDL_RenderFillRect(i64 %renderer278, i8* %cast287)
  %renderer288 = load i64, i64* %305, align 4
  store i64 %renderer288, i64* %187, align 4
  store i64 0, i64* %186, align 4
  br label %549

549:                                              ; preds = %547
  store i32 29, i32* %185, align 4
  store i32 153, i32* %184, align 4
  store i32 243, i32* %183, align 4
  br label %550

550:                                              ; preds = %549
  %b289 = load i32, i32* %183, align 4
  %g290 = load i32, i32* %184, align 4
  %r291 = load i32, i32* %185, align 4
  store i32 %r291, i32* %182, align 4
  store i32 %g290, i32* %181, align 4
  store i32 %b289, i32* %180, align 4
  br label %551

551:                                              ; preds = %550
  %button.height292 = load i32, i32* %314, align 4
  %button.width293 = load i32, i32* %315, align 4
  %button.y294 = load i32, i32* %316, align 4
  %button.x295 = load i32, i32* %317, align 4
  store i32 0, i32* %179, align 4
  store i32 0, i32* %178, align 4
  store i32 0, i32* %177, align 4
  store i32 %button.x295, i32* %176, align 4
  store i32 %button.y294, i32* %175, align 4
  store i32 %button.width293, i32* %174, align 4
  store i32 %button.height292, i32* %173, align 4
  store i32 0, i32* %172, align 4
  %552 = load i64, i64* %187, align 4
  store i64 %552, i64* %305, align 4
  br label %553

553:                                              ; preds = %551
  %renderer296 = load i64, i64* %187, align 4
  store i64 %renderer296, i64* %171, align 4
  br label %554

554:                                              ; preds = %553
  %renderer297 = load i64, i64* %171, align 4
  store i64 %renderer297, i64* %170, align 4
  %color.r298 = load i32, i32* %182, align 4
  store i32 %color.r298, i32* %169, align 4
  %color.g299 = load i32, i32* %181, align 4
  store i32 %color.g299, i32* %168, align 4
  %color.b300 = load i32, i32* %180, align 4
  store i32 %color.b300, i32* %167, align 4
  store i32 255, i32* %166, align 4
  br label %555

555:                                              ; preds = %554
  %renderer301 = load i64, i64* %170, align 4
  %r302 = load i32, i32* %169, align 4
  %g303 = load i32, i32* %168, align 4
  %b304 = load i32, i32* %167, align 4
  %a305 = load i32, i32* %166, align 4
  %556 = call i32 @SDL_SetRenderDrawColor(i64 %renderer301, i32 %r302, i32 %g303, i32 %b304, i32 %a305)
  store i32 %556, i32* %165, align 4
  %557 = load i32, i32* %165, align 4
  br label %558

558:                                              ; preds = %555
  %renderer306 = load i64, i64* %187, align 4
  store i64 %renderer306, i64* %164, align 4
  br label %559

559:                                              ; preds = %558
  %item.height307 = load i32, i32* %173, align 4
  %item.width308 = load i32, i32* %174, align 4
  %item.y309 = load i32, i32* %175, align 4
  %item.x310 = load i32, i32* %176, align 4
  store i32 %item.x310, i32* %163, align 4
  store i32 %item.y309, i32* %162, align 4
  store i32 %item.width308, i32* %161, align 4
  store i32 %item.height307, i32* %160, align 4
  br label %560

560:                                              ; preds = %559
  br label %561

561:                                              ; preds = %560
  %renderer311 = load i64, i64* %164, align 4
  %x312 = load i32, i32* %163, align 4
  %y313 = load i32, i32* %162, align 4
  %width314 = load i32, i32* %161, align 4
  %height315 = load i32, i32* %160, align 4
  %gep316 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %159, i32 0, i32 0
  store i32 %x312, i32* %gep316, align 4
  %gep317 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %159, i32 0, i32 1
  store i32 %y313, i32* %gep317, align 4
  %gep318 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %159, i32 0, i32 2
  store i32 %width314, i32* %gep318, align 4
  %gep319 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %159, i32 0, i32 3
  store i32 %height315, i32* %gep319, align 4
  %cast320 = bitcast { i32, i32, i32, i32 }* %159 to i8*
  %562 = call i32 @SDL_RenderFillRect(i64 %renderer311, i8* %cast320)
  %renderer321 = load i64, i64* %187, align 4
  store i64 %renderer321, i64* %158, align 4
  br label %563

563:                                              ; preds = %561
  store i1 false, i1* %157, align 1
  %564 = load i1, i1* %157, align 1
  br label %565

565:                                              ; preds = %563
  store i1 %564, i1* %156, align 1
  %566 = load i1, i1* %156, align 1
  br label %567

567:                                              ; preds = %565
  store i1 %566, i1* %155, align 1
  %568 = load i1, i1* %155, align 1
  br label %569

569:                                              ; preds = %567
  br label %572

570:                                              ; preds = %521
  br label %573

571:                                              ; preds = %537
  br label %574

572:                                              ; preds = %569
  br label %575

573:                                              ; preds = %570
  br label %680

574:                                              ; preds = %571
  br label %682

575:                                              ; preds = %572
  br label %684

junction:                                         ; preds = %469, %468, %467
  br label %576

576:                                              ; preds = %junction
  %renderer322 = load i64, i64* %338, align 4
  store i64 %renderer322, i64* %154, align 4
  br label %577

577:                                              ; preds = %576
  %renderer323 = load i64, i64* %154, align 4
  store i64 %renderer323, i64* %153, align 4
  br label %578

578:                                              ; preds = %577
  store i32 41, i32* %152, align 4
  store i32 128, i32* %151, align 4
  store i32 185, i32* %150, align 4
  br label %579

579:                                              ; preds = %578
  %b324 = load i32, i32* %150, align 4
  %g325 = load i32, i32* %151, align 4
  %r326 = load i32, i32* %152, align 4
  store i32 %r326, i32* %149, align 4
  store i32 %g325, i32* %148, align 4
  store i32 %b324, i32* %147, align 4
  br label %580

580:                                              ; preds = %579
  %button.height327 = load i32, i32* %308, align 4
  %581 = add i32 %button.height327, 2
  %button.width328 = load i32, i32* %309, align 4
  %button.y329 = load i32, i32* %310, align 4
  %button.x330 = load i32, i32* %311, align 4
  store i32 0, i32* %146, align 4
  store i32 0, i32* %145, align 4
  store i32 0, i32* %144, align 4
  store i32 %button.x330, i32* %143, align 4
  store i32 %button.y329, i32* %142, align 4
  store i32 %button.width328, i32* %141, align 4
  store i32 %581, i32* %140, align 4
  store i32 0, i32* %139, align 4
  store i32 41, i32* %138, align 4
  store i32 128, i32* %137, align 4
  store i32 185, i32* %136, align 4
  br label %582

582:                                              ; preds = %580
  %b331 = load i32, i32* %136, align 4
  %g332 = load i32, i32* %137, align 4
  %r333 = load i32, i32* %138, align 4
  store i32 %r333, i32* %135, align 4
  store i32 %g332, i32* %134, align 4
  store i32 %b331, i32* %133, align 4
  br label %583

583:                                              ; preds = %582
  %button.height334 = load i32, i32* %308, align 4
  %584 = add i32 %button.height334, 2
  %button.width335 = load i32, i32* %309, align 4
  %button.y336 = load i32, i32* %310, align 4
  %button.x337 = load i32, i32* %311, align 4
  store i32 0, i32* %132, align 4
  store i32 0, i32* %131, align 4
  store i32 0, i32* %130, align 4
  store i32 %button.x337, i32* %129, align 4
  store i32 %button.y336, i32* %128, align 4
  store i32 %button.width335, i32* %127, align 4
  store i32 %584, i32* %126, align 4
  store i32 0, i32* %125, align 4
  store i32 41, i32* %124, align 4
  store i32 128, i32* %123, align 4
  store i32 185, i32* %122, align 4
  br label %585

585:                                              ; preds = %583
  %b338 = load i32, i32* %122, align 4
  %g339 = load i32, i32* %123, align 4
  %r340 = load i32, i32* %124, align 4
  store i32 %r340, i32* %121, align 4
  store i32 %g339, i32* %120, align 4
  store i32 %b338, i32* %119, align 4
  br label %586

586:                                              ; preds = %585
  %button.height341 = load i32, i32* %308, align 4
  %587 = add i32 %button.height341, 2
  %button.width342 = load i32, i32* %309, align 4
  %button.y343 = load i32, i32* %310, align 4
  %button.x344 = load i32, i32* %311, align 4
  store i32 0, i32* %118, align 4
  store i32 0, i32* %117, align 4
  store i32 0, i32* %116, align 4
  store i32 %button.x344, i32* %115, align 4
  store i32 %button.y343, i32* %114, align 4
  store i32 %button.width342, i32* %113, align 4
  store i32 %587, i32* %112, align 4
  store i32 0, i32* %111, align 4
  br label %588

588:                                              ; preds = %586
  br label %condition347

condition347:                                     ; preds = %588
  %MouseOver345 = load i32, i32* @MouseOver, align 4
  %item.state346 = load i32, i32* %307, align 4
  %589 = icmp eq i32 %item.state346, %MouseOver345
  br i1 %589, label %590, label %condition350

590:                                              ; preds = %condition347
  br label %594

condition350:                                     ; preds = %condition347
  %Pressed348 = load i32, i32* @Pressed, align 4
  %item.state349 = load i32, i32* %307, align 4
  %591 = icmp eq i32 %item.state349, %Pressed348
  br i1 %591, label %592, label %593

592:                                              ; preds = %condition350
  br label %626

593:                                              ; preds = %condition350
  br label %642

594:                                              ; preds = %590
  br label %595

595:                                              ; preds = %594
  %renderer351 = load i64, i64* %153, align 4
  store i64 %renderer351, i64* %110, align 4
  br label %596

596:                                              ; preds = %595
  %renderer352 = load i64, i64* %110, align 4
  store i64 %renderer352, i64* %109, align 4
  %color.r353 = load i32, i32* %135, align 4
  store i32 %color.r353, i32* %108, align 4
  %color.g354 = load i32, i32* %134, align 4
  store i32 %color.g354, i32* %107, align 4
  %color.b355 = load i32, i32* %133, align 4
  store i32 %color.b355, i32* %106, align 4
  store i32 255, i32* %105, align 4
  br label %597

597:                                              ; preds = %596
  %renderer356 = load i64, i64* %109, align 4
  %r357 = load i32, i32* %108, align 4
  %g358 = load i32, i32* %107, align 4
  %b359 = load i32, i32* %106, align 4
  %a360 = load i32, i32* %105, align 4
  %598 = call i32 @SDL_SetRenderDrawColor(i64 %renderer356, i32 %r357, i32 %g358, i32 %b359, i32 %a360)
  store i32 %598, i32* %104, align 4
  %599 = load i32, i32* %104, align 4
  br label %600

600:                                              ; preds = %597
  %renderer361 = load i64, i64* %153, align 4
  store i64 %renderer361, i64* %103, align 4
  br label %601

601:                                              ; preds = %600
  %item.height362 = load i32, i32* %126, align 4
  %item.width363 = load i32, i32* %127, align 4
  %item.y364 = load i32, i32* %128, align 4
  %item.x365 = load i32, i32* %129, align 4
  store i32 %item.x365, i32* %102, align 4
  store i32 %item.y364, i32* %101, align 4
  store i32 %item.width363, i32* %100, align 4
  store i32 %item.height362, i32* %99, align 4
  br label %602

602:                                              ; preds = %601
  br label %603

603:                                              ; preds = %602
  %renderer366 = load i64, i64* %103, align 4
  %x367 = load i32, i32* %102, align 4
  %y368 = load i32, i32* %101, align 4
  %width369 = load i32, i32* %100, align 4
  %height370 = load i32, i32* %99, align 4
  %gep371 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %98, i32 0, i32 0
  store i32 %x367, i32* %gep371, align 4
  %gep372 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %98, i32 0, i32 1
  store i32 %y368, i32* %gep372, align 4
  %gep373 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %98, i32 0, i32 2
  store i32 %width369, i32* %gep373, align 4
  %gep374 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %98, i32 0, i32 3
  store i32 %height370, i32* %gep374, align 4
  %cast375 = bitcast { i32, i32, i32, i32 }* %98 to i8*
  %604 = call i32 @SDL_RenderFillRect(i64 %renderer366, i8* %cast375)
  %renderer376 = load i64, i64* %153, align 4
  store i64 %renderer376, i64* %97, align 4
  store i64 0, i64* %96, align 4
  br label %605

605:                                              ; preds = %603
  store i32 61, i32* %95, align 4
  store i32 174, i32* %94, align 4
  store i32 233, i32* %93, align 4
  br label %606

606:                                              ; preds = %605
  %b377 = load i32, i32* %93, align 4
  %g378 = load i32, i32* %94, align 4
  %r379 = load i32, i32* %95, align 4
  store i32 %r379, i32* %92, align 4
  store i32 %g378, i32* %91, align 4
  store i32 %b377, i32* %90, align 4
  br label %607

607:                                              ; preds = %606
  %button.height380 = load i32, i32* %308, align 4
  %button.width381 = load i32, i32* %309, align 4
  %button.y382 = load i32, i32* %310, align 4
  %button.x383 = load i32, i32* %311, align 4
  store i32 0, i32* %89, align 4
  store i32 0, i32* %88, align 4
  store i32 0, i32* %87, align 4
  store i32 %button.x383, i32* %86, align 4
  store i32 %button.y382, i32* %85, align 4
  store i32 %button.width381, i32* %84, align 4
  store i32 %button.height380, i32* %83, align 4
  store i32 0, i32* %82, align 4
  %608 = load i64, i64* %97, align 4
  store i64 %608, i64* %153, align 4
  br label %609

609:                                              ; preds = %607
  %renderer384 = load i64, i64* %97, align 4
  store i64 %renderer384, i64* %81, align 4
  br label %610

610:                                              ; preds = %609
  %renderer385 = load i64, i64* %81, align 4
  store i64 %renderer385, i64* %80, align 4
  %color.r386 = load i32, i32* %92, align 4
  store i32 %color.r386, i32* %79, align 4
  %color.g387 = load i32, i32* %91, align 4
  store i32 %color.g387, i32* %78, align 4
  %color.b388 = load i32, i32* %90, align 4
  store i32 %color.b388, i32* %77, align 4
  store i32 255, i32* %76, align 4
  br label %611

611:                                              ; preds = %610
  %renderer389 = load i64, i64* %80, align 4
  %r390 = load i32, i32* %79, align 4
  %g391 = load i32, i32* %78, align 4
  %b392 = load i32, i32* %77, align 4
  %a393 = load i32, i32* %76, align 4
  %612 = call i32 @SDL_SetRenderDrawColor(i64 %renderer389, i32 %r390, i32 %g391, i32 %b392, i32 %a393)
  store i32 %612, i32* %75, align 4
  %613 = load i32, i32* %75, align 4
  br label %614

614:                                              ; preds = %611
  %renderer394 = load i64, i64* %97, align 4
  store i64 %renderer394, i64* %74, align 4
  br label %615

615:                                              ; preds = %614
  %item.height395 = load i32, i32* %83, align 4
  %item.width396 = load i32, i32* %84, align 4
  %item.y397 = load i32, i32* %85, align 4
  %item.x398 = load i32, i32* %86, align 4
  store i32 %item.x398, i32* %73, align 4
  store i32 %item.y397, i32* %72, align 4
  store i32 %item.width396, i32* %71, align 4
  store i32 %item.height395, i32* %70, align 4
  br label %616

616:                                              ; preds = %615
  br label %617

617:                                              ; preds = %616
  %renderer399 = load i64, i64* %74, align 4
  %x400 = load i32, i32* %73, align 4
  %y401 = load i32, i32* %72, align 4
  %width402 = load i32, i32* %71, align 4
  %height403 = load i32, i32* %70, align 4
  %gep404 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %69, i32 0, i32 0
  store i32 %x400, i32* %gep404, align 4
  %gep405 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %69, i32 0, i32 1
  store i32 %y401, i32* %gep405, align 4
  %gep406 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %69, i32 0, i32 2
  store i32 %width402, i32* %gep406, align 4
  %gep407 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %69, i32 0, i32 3
  store i32 %height403, i32* %gep407, align 4
  %cast408 = bitcast { i32, i32, i32, i32 }* %69 to i8*
  %618 = call i32 @SDL_RenderFillRect(i64 %renderer399, i8* %cast408)
  %renderer409 = load i64, i64* %97, align 4
  store i64 %renderer409, i64* %68, align 4
  br label %619

619:                                              ; preds = %617
  store i1 false, i1* %67, align 1
  %620 = load i1, i1* %67, align 1
  br label %621

621:                                              ; preds = %619
  store i1 %620, i1* %66, align 1
  %622 = load i1, i1* %66, align 1
  br label %623

623:                                              ; preds = %621
  store i1 %622, i1* %65, align 1
  %624 = load i1, i1* %65, align 1
  br label %625

625:                                              ; preds = %623
  br label %674

626:                                              ; preds = %592
  br label %627

627:                                              ; preds = %626
  %renderer410 = load i64, i64* %153, align 4
  store i64 %renderer410, i64* %64, align 4
  br label %628

628:                                              ; preds = %627
  %renderer411 = load i64, i64* %64, align 4
  store i64 %renderer411, i64* %63, align 4
  %color.r412 = load i32, i32* %149, align 4
  store i32 %color.r412, i32* %62, align 4
  %color.g413 = load i32, i32* %148, align 4
  store i32 %color.g413, i32* %61, align 4
  %color.b414 = load i32, i32* %147, align 4
  store i32 %color.b414, i32* %60, align 4
  store i32 255, i32* %59, align 4
  br label %629

629:                                              ; preds = %628
  %renderer415 = load i64, i64* %63, align 4
  %r416 = load i32, i32* %62, align 4
  %g417 = load i32, i32* %61, align 4
  %b418 = load i32, i32* %60, align 4
  %a419 = load i32, i32* %59, align 4
  %630 = call i32 @SDL_SetRenderDrawColor(i64 %renderer415, i32 %r416, i32 %g417, i32 %b418, i32 %a419)
  store i32 %630, i32* %58, align 4
  %631 = load i32, i32* %58, align 4
  br label %632

632:                                              ; preds = %629
  %renderer420 = load i64, i64* %153, align 4
  store i64 %renderer420, i64* %57, align 4
  br label %633

633:                                              ; preds = %632
  %item.height421 = load i32, i32* %140, align 4
  %item.width422 = load i32, i32* %141, align 4
  %item.y423 = load i32, i32* %142, align 4
  %item.x424 = load i32, i32* %143, align 4
  store i32 %item.x424, i32* %56, align 4
  store i32 %item.y423, i32* %55, align 4
  store i32 %item.width422, i32* %54, align 4
  store i32 %item.height421, i32* %53, align 4
  br label %634

634:                                              ; preds = %633
  br label %635

635:                                              ; preds = %634
  %renderer425 = load i64, i64* %57, align 4
  %x426 = load i32, i32* %56, align 4
  %y427 = load i32, i32* %55, align 4
  %width428 = load i32, i32* %54, align 4
  %height429 = load i32, i32* %53, align 4
  %gep430 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %52, i32 0, i32 0
  store i32 %x426, i32* %gep430, align 4
  %gep431 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %52, i32 0, i32 1
  store i32 %y427, i32* %gep431, align 4
  %gep432 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %52, i32 0, i32 2
  store i32 %width428, i32* %gep432, align 4
  %gep433 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %52, i32 0, i32 3
  store i32 %height429, i32* %gep433, align 4
  %cast434 = bitcast { i32, i32, i32, i32 }* %52 to i8*
  %636 = call i32 @SDL_RenderFillRect(i64 %renderer425, i8* %cast434)
  %renderer435 = load i64, i64* %153, align 4
  store i64 %renderer435, i64* %51, align 4
  br label %637

637:                                              ; preds = %635
  store i1 false, i1* %50, align 1
  %638 = load i1, i1* %50, align 1
  br label %639

639:                                              ; preds = %637
  store i1 %638, i1* %49, align 1
  %640 = load i1, i1* %49, align 1
  br label %641

641:                                              ; preds = %639
  br label %675

642:                                              ; preds = %593
  br label %643

643:                                              ; preds = %642
  %renderer436 = load i64, i64* %153, align 4
  store i64 %renderer436, i64* %48, align 4
  br label %644

644:                                              ; preds = %643
  %renderer437 = load i64, i64* %48, align 4
  store i64 %renderer437, i64* %47, align 4
  %color.r438 = load i32, i32* %121, align 4
  store i32 %color.r438, i32* %46, align 4
  %color.g439 = load i32, i32* %120, align 4
  store i32 %color.g439, i32* %45, align 4
  %color.b440 = load i32, i32* %119, align 4
  store i32 %color.b440, i32* %44, align 4
  store i32 255, i32* %43, align 4
  br label %645

645:                                              ; preds = %644
  %renderer441 = load i64, i64* %47, align 4
  %r442 = load i32, i32* %46, align 4
  %g443 = load i32, i32* %45, align 4
  %b444 = load i32, i32* %44, align 4
  %a445 = load i32, i32* %43, align 4
  %646 = call i32 @SDL_SetRenderDrawColor(i64 %renderer441, i32 %r442, i32 %g443, i32 %b444, i32 %a445)
  store i32 %646, i32* %42, align 4
  %647 = load i32, i32* %42, align 4
  br label %648

648:                                              ; preds = %645
  %renderer446 = load i64, i64* %153, align 4
  store i64 %renderer446, i64* %41, align 4
  br label %649

649:                                              ; preds = %648
  %item.height447 = load i32, i32* %112, align 4
  %item.width448 = load i32, i32* %113, align 4
  %item.y449 = load i32, i32* %114, align 4
  %item.x450 = load i32, i32* %115, align 4
  store i32 %item.x450, i32* %40, align 4
  store i32 %item.y449, i32* %39, align 4
  store i32 %item.width448, i32* %38, align 4
  store i32 %item.height447, i32* %37, align 4
  br label %650

650:                                              ; preds = %649
  br label %651

651:                                              ; preds = %650
  %renderer451 = load i64, i64* %41, align 4
  %x452 = load i32, i32* %40, align 4
  %y453 = load i32, i32* %39, align 4
  %width454 = load i32, i32* %38, align 4
  %height455 = load i32, i32* %37, align 4
  %gep456 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %36, i32 0, i32 0
  store i32 %x452, i32* %gep456, align 4
  %gep457 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %36, i32 0, i32 1
  store i32 %y453, i32* %gep457, align 4
  %gep458 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %36, i32 0, i32 2
  store i32 %width454, i32* %gep458, align 4
  %gep459 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %36, i32 0, i32 3
  store i32 %height455, i32* %gep459, align 4
  %cast460 = bitcast { i32, i32, i32, i32 }* %36 to i8*
  %652 = call i32 @SDL_RenderFillRect(i64 %renderer451, i8* %cast460)
  %renderer461 = load i64, i64* %153, align 4
  store i64 %renderer461, i64* %35, align 4
  store i64 0, i64* %34, align 4
  br label %653

653:                                              ; preds = %651
  store i32 29, i32* %33, align 4
  store i32 153, i32* %32, align 4
  store i32 243, i32* %31, align 4
  br label %654

654:                                              ; preds = %653
  %b462 = load i32, i32* %31, align 4
  %g463 = load i32, i32* %32, align 4
  %r464 = load i32, i32* %33, align 4
  store i32 %r464, i32* %30, align 4
  store i32 %g463, i32* %29, align 4
  store i32 %b462, i32* %28, align 4
  br label %655

655:                                              ; preds = %654
  %button.height465 = load i32, i32* %308, align 4
  %button.width466 = load i32, i32* %309, align 4
  %button.y467 = load i32, i32* %310, align 4
  %button.x468 = load i32, i32* %311, align 4
  store i32 0, i32* %27, align 4
  store i32 0, i32* %26, align 4
  store i32 0, i32* %25, align 4
  store i32 %button.x468, i32* %24, align 4
  store i32 %button.y467, i32* %23, align 4
  store i32 %button.width466, i32* %22, align 4
  store i32 %button.height465, i32* %21, align 4
  store i32 0, i32* %20, align 4
  %656 = load i64, i64* %35, align 4
  store i64 %656, i64* %153, align 4
  br label %657

657:                                              ; preds = %655
  %renderer469 = load i64, i64* %35, align 4
  store i64 %renderer469, i64* %19, align 4
  br label %658

658:                                              ; preds = %657
  %renderer470 = load i64, i64* %19, align 4
  store i64 %renderer470, i64* %18, align 4
  %color.r471 = load i32, i32* %30, align 4
  store i32 %color.r471, i32* %17, align 4
  %color.g472 = load i32, i32* %29, align 4
  store i32 %color.g472, i32* %16, align 4
  %color.b473 = load i32, i32* %28, align 4
  store i32 %color.b473, i32* %15, align 4
  store i32 255, i32* %14, align 4
  br label %659

659:                                              ; preds = %658
  %renderer474 = load i64, i64* %18, align 4
  %r475 = load i32, i32* %17, align 4
  %g476 = load i32, i32* %16, align 4
  %b477 = load i32, i32* %15, align 4
  %a478 = load i32, i32* %14, align 4
  %660 = call i32 @SDL_SetRenderDrawColor(i64 %renderer474, i32 %r475, i32 %g476, i32 %b477, i32 %a478)
  store i32 %660, i32* %13, align 4
  %661 = load i32, i32* %13, align 4
  br label %662

662:                                              ; preds = %659
  %renderer479 = load i64, i64* %35, align 4
  store i64 %renderer479, i64* %12, align 4
  br label %663

663:                                              ; preds = %662
  %item.height480 = load i32, i32* %21, align 4
  %item.width481 = load i32, i32* %22, align 4
  %item.y482 = load i32, i32* %23, align 4
  %item.x483 = load i32, i32* %24, align 4
  store i32 %item.x483, i32* %11, align 4
  store i32 %item.y482, i32* %10, align 4
  store i32 %item.width481, i32* %9, align 4
  store i32 %item.height480, i32* %8, align 4
  br label %664

664:                                              ; preds = %663
  br label %665

665:                                              ; preds = %664
  %renderer484 = load i64, i64* %12, align 4
  %x485 = load i32, i32* %11, align 4
  %y486 = load i32, i32* %10, align 4
  %width487 = load i32, i32* %9, align 4
  %height488 = load i32, i32* %8, align 4
  %gep489 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %7, i32 0, i32 0
  store i32 %x485, i32* %gep489, align 4
  %gep490 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %7, i32 0, i32 1
  store i32 %y486, i32* %gep490, align 4
  %gep491 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %7, i32 0, i32 2
  store i32 %width487, i32* %gep491, align 4
  %gep492 = getelementptr inbounds { i32, i32, i32, i32 }, { i32, i32, i32, i32 }* %7, i32 0, i32 3
  store i32 %height488, i32* %gep492, align 4
  %cast493 = bitcast { i32, i32, i32, i32 }* %7 to i8*
  %666 = call i32 @SDL_RenderFillRect(i64 %renderer484, i8* %cast493)
  %renderer494 = load i64, i64* %35, align 4
  store i64 %renderer494, i64* %6, align 4
  br label %667

667:                                              ; preds = %665
  store i1 false, i1* %5, align 1
  %668 = load i1, i1* %5, align 1
  br label %669

669:                                              ; preds = %667
  store i1 %668, i1* %4, align 1
  %670 = load i1, i1* %4, align 1
  br label %671

671:                                              ; preds = %669
  store i1 %670, i1* %3, align 1
  %672 = load i1, i1* %3, align 1
  br label %673

673:                                              ; preds = %671
  br label %676

674:                                              ; preds = %625
  br label %677

675:                                              ; preds = %641
  br label %678

676:                                              ; preds = %673
  br label %679

677:                                              ; preds = %674
  br label %686

678:                                              ; preds = %675
  br label %689

679:                                              ; preds = %676
  br label %692

680:                                              ; preds = %573
  br label %681

681:                                              ; preds = %680
  br label %695

682:                                              ; preds = %574
  br label %683

683:                                              ; preds = %682
  br label %698

684:                                              ; preds = %575
  br label %685

685:                                              ; preds = %684
  br label %701

686:                                              ; preds = %677
  br label %687

687:                                              ; preds = %686
  %index495 = load i64, i64* %337, align 4
  %688 = add i64 %index495, 1
  store i64 %688, i64* %337, align 4
  %renderer496 = load i64, i64* %338, align 4
  store i64 %renderer496, i64* %338, align 4
  br label %458

689:                                              ; preds = %678
  br label %690

690:                                              ; preds = %689
  %index497 = load i64, i64* %337, align 4
  %691 = add i64 %index497, 1
  store i64 %691, i64* %337, align 4
  %renderer498 = load i64, i64* %338, align 4
  store i64 %renderer498, i64* %338, align 4
  br label %458

692:                                              ; preds = %679
  br label %693

693:                                              ; preds = %692
  %index499 = load i64, i64* %337, align 4
  %694 = add i64 %index499, 1
  store i64 %694, i64* %337, align 4
  %renderer500 = load i64, i64* %338, align 4
  store i64 %renderer500, i64* %338, align 4
  br label %458

695:                                              ; preds = %681
  br label %696

696:                                              ; preds = %695
  %renderer501 = load i64, i64* %405, align 4
  store i64 %renderer501, i64* %2, align 4
  br label %697

697:                                              ; preds = %696
  %renderer502 = load i64, i64* %2, align 4
  call void @SDL_RenderPresent(i64 %renderer502)
  br label %704

698:                                              ; preds = %683
  br label %699

699:                                              ; preds = %698
  %renderer503 = load i64, i64* %405, align 4
  store i64 %renderer503, i64* %1, align 4
  br label %700

700:                                              ; preds = %699
  %renderer504 = load i64, i64* %1, align 4
  call void @SDL_RenderPresent(i64 %renderer504)
  br label %706

701:                                              ; preds = %685
  br label %702

702:                                              ; preds = %701
  %renderer505 = load i64, i64* %405, align 4
  store i64 %renderer505, i64* %0, align 4
  br label %703

703:                                              ; preds = %702
  %renderer506 = load i64, i64* %0, align 4
  call void @SDL_RenderPresent(i64 %renderer506)
  br label %708

704:                                              ; preds = %697
  %705 = load i64, i64* %406, align 4
  store i64 %705, i64* %408, align 4
  br label %428

706:                                              ; preds = %700
  %707 = load i64, i64* %406, align 4
  store i64 %707, i64* %408, align 4
  br label %428

708:                                              ; preds = %703
  %709 = load i64, i64* %406, align 4
  store i64 %709, i64* %408, align 4
  br label %428
}

declare i32 @SDL_Init(i32)

declare i64 @SDL_CreateWindow(i8*, i32, i32, i32, i32, i32)

declare i64 @SDL_CreateRenderer(i64, i32, i32)

declare i32 @SDL_SetRenderDrawColor(i64, i32, i32, i32, i32)

declare i32 @SDL_RenderClear(i64)

declare i8* @SDL_WaitEventEx(i8*)

declare i32 @SDL_RenderFillRect(i64, i8*)

declare void @SDL_RenderPresent(i64)
